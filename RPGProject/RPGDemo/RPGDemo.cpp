#include "common.h"

#include "GameLoop.h"
#include "Win32Platform.h"
#include "SysMsgHandler.h"

#include "CFlyCommon.h"
#include "PhysicsSystem.h"
#include "CUISystem.h"
#include "TEventSystem.h"
#include "TItemSystem.h"
#include "ConsoleSystem.h"

#include "ActorMovement.h"
#include "CActorModel.h"
#include "TriangleMeshData.h"
#include "SpatialComponent.h"

#include "ui/CBloodBar.h"
#include "ui/CVitalStateUI.h"
#include "ui/CItemBagPanel.h"
#include "ui/CEquipFrame.h"
#include "ui/CMiniMapUI.h"
#include "ui/CTextButton.h"
#include "ui/CGameUI.h"

#include "TPlayerControl.h"

#include "TAblilityProp.h"
#include "AbilityProp.h"

#include "TResManager.h"
#include "TRoleTemplate.h"

#include "TMessageShow.h"
#include "ConsoleSystem.h"

#include "CSceneLevel.h"
#include "CCamera.h"

#include "UtilityGlobal.h"
#include "EventType.h"

#include "Platform.h"
#include "RenderSystem.h"
#include "IScriptTable.h"
#include "GameFramework.h"
#include "CGameMod.h"


ConsoleSystem gConsoleSystem;

#include "CNPCBase.h"
#include "CHero.h"

GlobalVal g_GlobalVal( 30 );

class EntityTemplateManager
{


};

class ControlComponent;






#include <memory>

class CProfileViewer : public TProfileViewer< CProfileViewer >
{
public:

	void onRoot     ( SampleNode* node )
	{
		double time_since_reset = ProfileSystem::getInstance().getTimeSinceReset();
		msgShow.push( "--- Profiling: %s (total running time: %.3f ms) ---" , 
			node->getName() , time_since_reset );
	}

	void onNode     ( SampleNode* node , double parentTime )
	{
		float tf = node->getTotalTime()  / (double)ProfileSystem::getInstance().getFrameCountSinceReset();
		msgShow.push( "|-> %d -- %s (%.2f %%) :: %.3f ms / frame  (%.3f (1e-5ms/call) (%d calls)",
			++mIdxChild , node->getName() ,
			parentTime > CLOCK_EPSILON ? ( node->getTotalTime()  / parentTime ) * 100 : 0.f ,
			tf , 100000 * ( tf / node->getTotalCalls() ) ,
			node->getTotalCalls() );
	}

	bool onEnterChild( SampleNode* node )
	{ 

		mIdxChild = 0;

		msgShow.shiftPos( 20 , 0 );

		return true; 
	}
	void onEnterParent( SampleNode* node , int numChildren , double accTime )
	{
		if ( numChildren )
		{
			double time;
			if ( node->getParent() != NULL )
				time = node->getTotalTime();
			else
				time = ProfileSystem::getInstance().getTimeSinceReset();

			double delta = time - accTime;
			msgShow.push( "|-> %s (%.3f %%) :: %.3f ms / frame", "Other",
				// (min(0, time_since_reset - totalTime) / time_since_reset) * 100);
				( time > CLOCK_EPSILON ) ? ( delta / time * 100 ) : 0.f  , 
				delta / (double)ProfileSystem::getInstance().getFrameCountSinceReset() );
			msgShow.push( "-------------------------------------------------" );
		}

		msgShow.shiftPos( -20 , 0 );
	}


	void showText()
	{
		msgShow.setPos( 10 , 40 );
		msgShow.start();

		visit();
		msgShow.finish();
	}

	int    mIdxChild;

	TMessageShow msgShow;
}g_ProfileViewer;


extern Vec3D dbgMoveDir;

class RPGProjectGame : public GameLoopT< RPGProjectGame , Win32Platform >
	                 , public WinFrameT < RPGProjectGame >
	                 , public SysMsgHandlerT< RPGProjectGame >
{
public:
	RPGProjectGame()
		:mDefultData( 1 , 100 , 100 , 0 ,0,0,0 , 0 )
	{
		mGameUI = nullptr;
	}

	void onIdle( long time )        
	{
		onRender();
	}

	CEntity* createBoxEntity( Vec3D const& pos )
	{
		static EntitySpawnParams params;

		if ( !params.entityClass )
		{
			params.entityClass = gEnv->entitySystem->findClass( "RigidObject" );
			if ( !params.entityClass )
				return NULL;

			params.propertyTable = gEnv->scriptSystem->createScriptTable();
			params.propertyTable->pushValue( "modelName" , "box" );
			params.helper = &mHelper;
		}

		params.pos = pos;
		return gEnv->entitySystem->spawnEntity( params );
	}


	template< class T >
	class TGameObjectFactory : public IGameObjectExtensionFactory
	{
	public:
		virtual T*   create(){  return new T();  }
		virtual void release(){ delete this; }
		static  TGameObjectFactory* createFactory(){ return new TGameObjectFactory;  }
	};
	template< class ES >
	ES* getEntityScript()
	{
		static ES es;
		return &es;
	}


	GameFramework* mGameFramework;
	IGameMod*      mGameMod;
	bool onInit()                   
	{
		int w = 1024;
		int h = 768;

		if ( !WinFrame::create( TEXT("RPG Project") , w , h , &SysMsgHandler::MsgProc ) )
			return false;

		SSystemInitParams sysParams;
		sysParams.hInstance    = ::GetModuleHandle( NULL );
		sysParams.hWnd         = getHWnd();
		sysParams.bufferWidth  = w;
		sysParams.bufferHeight = h;
		sysParams.platform  = new CWinPlatform( sysParams.hInstance , sysParams.hWnd );

		mGameFramework = new GameFramework;
		if ( !mGameFramework->init( sysParams ) )
			return false;

		gEnv->OSPlatform = sysParams.platform;

		mGameMod = new CGameMod;
		if ( !mGameMod->init( mGameFramework ) )
			return false;
		gEnv->gameMod = mGameMod;

		g_ProfileViewer.msgShow.mWorld = gEnv->renderSystem->getCFWorld();
		mViewport = gEnv->renderSystem->getScreenViewport();
		mCFWorld  = gEnv->renderSystem->getCFWorld();

		mSceneLevel = mGameFramework->getLevelManager()->createEmptyLevel( "BT_LOWER" , SLF_USE_MAP | SLF_USE_MAP_NAVMESH );

		if ( !mSceneLevel->loadSceneMap("BT_LOWER") )
			return false;

		if ( !mSceneLevel->prepare() )
			return false;
		mSceneLevel->active();

		mCFScene = mSceneLevel->getRenderScene();
		TResManager::getInstance().setCurScene( mCFScene );

		mCFCamera = mCFScene->createCamera();
		mCFCamera->setAspect( float(  w ) / h );
		mCFCamera->translate(  Vec3D(0,0,100) , CFly::CFTO_GLOBAL );
		mCFCamera->rotate(  CFly::CF_AXIS_Y , Math::Deg2Rad( 180 ) , CFly::CFTO_LOCAL );
		mCFCamera->setNear( 10 );
		mCFCamera->setFar( 5000 );

		mPhyWorld = mSceneLevel->getPhysicsWorld();

		mHelper.cfWorld    = mCFWorld;
		mHelper.sceneLevel = mSceneLevel;

		{
			IEntityClass* actorClass = gEnv->entitySystem->findClass( "Actor:NPC" ); 

			Vec3D worldPos(0,430,500);

			IScriptTable* scriptTable = gEnv->scriptSystem->createScriptTable();
			scriptTable->pushValue( "roleID" , (unsigned)2 );

			EntitySpawnParams params;
			params.propertyTable;
			params.pos           = worldPos;
			params.entityClass   = actorClass; 
			params.propertyTable = scriptTable;
			params.helper        = &mHelper;

			//CEntity* entity = gEnv->entitySystem->spawnEntity( params );

		}

		{

			IEntityClass* actorClass = gEnv->entitySystem->findClass( "Actor:Hero" ); 

			Vec3D worldPos(0,530,500);

			IScriptTable* scriptTable = gEnv->scriptSystem->createScriptTable();
			scriptTable->pushValue( "roleID" , (unsigned)ROLE_HERO );

			EntitySpawnParams params;
			params.propertyTable;
			params.pos           = worldPos;
			params.entityClass   = actorClass; 
			params.propertyTable = scriptTable;
			params.helper        = &mHelper;

			CEntity* entity = gEnv->entitySystem->spawnEntity( params );
			
			mSpatialComp  = entity->getComponentT< ISpatialComponent >( COMP_SPATIAL );
			GameObject* gameObject = entity->getComponentT< GameObject >( COMP_GAME_OBJECT );


			mPlayer = static_cast< CHero* >( gameObject->queryExtension( "Hero" ) );
			mMovement   = mPlayer->getMovement();
			mActorModel = mPlayer->getModel();

			for( int i = 0 ; i < 10 ; ++i )
				mPlayer->addItem( i );

	
			mGameUI = new CGameUI( mPlayer );
			mGameUI->changeLevel( mSceneLevel );
			mGameUI->resizeScreenArea( w , h );

			mPlayerControl = new PlayerControl( &mCamera , mGameUI , mPlayer );
			mPlayerControl->changeLevel( mSceneLevel );

			//CTextButton* button = new CTextButton("hello" , 100 , TVec2D(100,200) , nullptr );
			//CUISystem::getInstance().addWidget( button );

			//CMiniMapUI* miniMap = new CMiniMapUI( TVec2D(400 , 0 ) );
			//CUISystem::getInstance().addWidget( miniMap );

			//miniMap->setFollowActor( mLogicComp );
			//miniMap->setLevel( mSceneLevel );

			//CFrameUI* panel = new CFrameUI( TVec2D(10,10) , TVec2D( 300 , 300 ) , nullptr );
			//CUISystem::getInstance().addWidget( panel );
			//{
			//	CItemBagPanel* panel = new CItemBagPanel( mLogicComp , TVec2D(400 , 400 ) );
			//	CUISystem::getInstance().addWidget( panel );
			//}

			//CPanelUI* panel2 = new CPanelUI( TVec2D(10,10) , TVec2D( 200 , 200 ) , nullptr );
			//CUISystem::getInstance().addWidget( panel2 );

			//CVitalStateUI* vitalStateUI = new CVitalStateUI( TVec2D(0,0) , PropComp );
			//CUISystem::getInstance().addWidget( vitalStateUI );
			//mBloodBarComp = vitalStateUI->m_HPBar;
		}


		{

			//RigidComponent* phyicalComp = mPhyWorld->createRigidComponent( mSceneLevel->getSceneMap().getTerrainShape() , 0 );

			//data.createDebugGemoWithNormalColor( obj , NULL );
			//obj->setRenderMode( CFRM_WIREFRAME );
		}


		//mPhyWorld->getDebugDrawer()->setObject(obj);
		//mPhyWorld->enableDebugDraw( true );

		//for ( int i = 0 ; i < 20 ; ++i )
		//{
		//	mCFWorld->setDir( CFly::DIR_TEXTURE , "Data/UI/Skills" );
		//	float color[4] = { 1 , 1 , 1 , 0.5 };
		//	Sprite* sprite = CUISystem::getInstance().createSprite();
		//	sprite->createRectArea( 0 , 0 , 64 , 64 ,"Spell_Fire_FlameShock" );

		//	//mCFWorld->setDir( CFly::DIR_TEXTURE , "Data/UI/Panel" );
		//	//sprite = CUISystem::getInstance().createSprite( TVec2D(100 , 36) , "panel_side_bottom"  );
		//	//sprite->setRectArea( nullptr , 64 , 64 , "Spell_Fire_FlameShock" , 0 , nullptr , false , 0 , 0 , 0 , CF_FILTER_POINT );
		//	sprite->setLocalPosition( Vec3D( 100 + 10 * i  ,100, 10 - 1 * i   ) );
		//	//sprite->setRenderOption( CFly::CFRO_CULL_FACE , CFly::CF_CULL_NONE );
		//	sprite->setRenderOption( CFly::CFRO_ALPHA_BLENGING , TRUE );
		//}

		//for ( int i = 0 ; i < 10 ; ++i )
		//{
		//	mCFWorld->setDir( CFly::DIR_TEXTURE , "Data/UI/Skills" );
		//	float color[4] = { 1 , 1 , 1 , 0.5 };
		//	Sprite* sprite = CUISystem::getInstance().createSprite();
		//	sprite->createRectArea( 0 , 0 , 64 , 64 ,"Spell_Fire_FlameShock" , 0 , - 1 * i  );

		//	//mCFWorld->setDir( CFly::DIR_TEXTURE , "Data/UI/Panel" );
		//	//sprite = CUISystem::getInstance().createSprite( TVec2D(100 , 36) , "panel_side_bottom"  );
		//	//sprite->setRectArea( nullptr , 64 , 64 , "Spell_Fire_FlameShock" , 0 , nullptr , false , 0 , 0 , 0 , CF_FILTER_POINT );
		//	sprite->setLocalPosition( Vec3D( 100 + 10 * i  ,200, -10 ) );
		//	//sprite->setRenderOption( CFly::CFRO_CULL_FACE , CFly::CF_CULL_NONE );
		//	sprite->setRenderOption( CFly::CFRO_ALPHA_BLENGING , TRUE );
		//}



		//m_mask = new CDMask( TVec2D(64,64) , CUISystem::getInstance().mCFScene );
		//m_mask->setState( 5 , 10 );

		mCamera.setPosition( Vec3D(0,0,0) );
		mCamera.setViewDir( Vec3D(1,0,0) );
		mCamControl = new FristViewCamControl( &mCamera );

		return true; 
	}

	GameObjectInitHelper mHelper;

	CDMask*    mCDMask;
	CBloodBar* mBloodBarComp ;

	SAbilityPropData mDefultData;
	TriangleMeshData data;

	CGameUI*  mGameUI;

	void onEnd()                    
	{
		delete mGameFramework;
		delete mGameMod;
	}

	typedef std::vector< CEntity* > EntityList;
	long onUpdate( long shouldTime )
	{ 
		ProfileSystem::getInstance().incrementFrameCount();

		if ( mCDMask )
			mCDMask->update();

		PROFILE_ENTRY( "onUpdate" );
		long updateTime = 1000 / 30;

		if ( mPlayerControl )
		{
			mPlayerControl->updateFrame();
		}
		else
		{
			if ( GetAsyncKeyState( VK_LEFT ) )
				mSpatialComp->rotate( Quat::Rotate( Vec3D( 0,0,1 ) , -0.03 ) );
			if ( GetAsyncKeyState(VK_RIGHT ) )
				mSpatialComp->rotate( Quat::Rotate( Vec3D( 0,0,1 ) , 0.03 ) );

			if ( GetAsyncKeyState( VK_UP ) )
			{
				Vec3D frontDir(0,-1,0);
				frontDir = rotate( frontDir , mSpatialComp->getTransform() );

				mMovement->setVelocityForTimeInterval( 500 * frontDir , shouldTime / 1000.0f);

				mActorModel->changeAction( ANIM_RUN );
			}


			if ( GetAsyncKeyState( VK_SPACE ) )
			{
				mMovement->jump();
			}
		}

		gEnv->gameMod->update( shouldTime );

		mSceneLevel->update( updateTime );

		if ( mGameUI )
			mGameUI->update( updateTime );

		//mPhyWorld->renderDebugDraw();


		mTestEntity.update( shouldTime );



		
		Mat4x4 trans;
		Vec3D pos = mCamera.getPosition();
		trans.setTransform( mCamera.getPosition() , mCamera.getOrientation() );

		mCFCamera->setLookAt( mCamera.getPosition() , mCamera.getLookPos() , mCamera.transLocalDir( Vec3D(0,0,1) ) );
		//mCFCamera->applyTransform( CFly::CFTO_REPLACE  , trans );
		//mCFCamera->rotate( CFly::CFTO_REPLACE , mCamera.getOrientation() );
		//mCFCamera->rotate( CFly::CFTO_LOCAL , CFly::CF_AXIS_X , Math::Deg2Rad(90) );
		//mCFCamera->rotate( CFly::CFTO_LOCAL , CFly::CF_AXIS_Y , Math::Deg2Rad(90) );


		return shouldTime; 
	}


	void onRender()                 
	{
		PROFILE_ENTRY("onRender");

		{
			PROFILE_ENTRY("RenderScene");
			mCFScene->render( mCFCamera , mViewport );
			CUISystem::getInstance().render();
		}

		{
			PROFILE_ENTRY("drawText");
			//g_ProfileViewer.showText();

			static int   m_frameCount = 0;
			static float time = GetTickCount();
			static float m_fps = 0;
			++m_frameCount;
			if ( m_frameCount > 100 )
			{
				m_fps = 1000.0f * ( m_frameCount ) / ( GetTickCount() - time );
				time = GetTickCount();
				m_frameCount = 0;
			}
			mCFWorld->beginMessage();
			Vec3D front = mPlayer->getFaceDir();
			Vec3D pos   = mPlayer->getPosition();
			Vec3D moveDir = mPlayerControl->moveDir;
			drawText( 10 , 10 , "fps = %f " , m_fps );
			drawText( 10 , 25 , "PP = (%f %f %f)" , pos.x , pos.y , pos.z );
			drawText( 10 , 40 , "PF = (%f %f %f)" , front.x , front.y , front.z );
			drawText( 10 , 55 , "MD = (%f %f %f)" , dbgMoveDir.x , dbgMoveDir.y , dbgMoveDir.z );
			mCFWorld->endMessage();
		}

		{
			PROFILE_ENTRY("SwapBuffer");
			mCFWorld->swapBuffers();
		}

	}

	void drawText( int x, int y , char const* fmt , ... )
	{
		char str[256];
		va_list args;
		va_start( args , fmt );
		vsprintf_s( str , fmt , args );
		mCFWorld->showMessage( x , y , str , 255 , 255 , 255 );
		va_end( args );
	}


	bool onMouse( MouseMsg& msg )
	{
		static Vec2i pos;

		if ( !CUISystem::getInstance().processMouseEvent( msg ) )
			return false;

		if ( msg.onLeftDown() )
		{
			pos = msg.getPos();
		}
		else if ( msg.isDraging() && msg.isLeftDown() )
		{
			//mCFCamera->rotate( CFly::CFTO_LOCAL , CFly::CF_AXIS_Y , 0.01 *( pos.x - msg.getPos().x ) );
			//mCFCamera->rotate( CFly::CFTO_LOCAL , CFly::CF_AXIS_X , -0.01 *( pos.y - msg.getPos().y ) );

			Vec2i offset = msg.getPos() - pos;
			mCamControl->rotateByMouse( offset.x , offset.y );
			pos = msg.getPos();
		}
		return true;
	}

	bool onKey( unsigned key , bool isDown )
	{
		if ( !isDown )
			return false;

		switch( key )
		{
		case VK_ESCAPE: setLoopOver( true ); break;
		//case 'W': mCFCamera->translate( CFly::CFTO_LOCAL , Vec3D( 0,0,10 ) ); break;
		//case 'S': mCFCamera->translate( CFly::CFTO_LOCAL , Vec3D( 0,0,-10 ) ); break;
		//case 'A': mCFCamera->translate( CFly::CFTO_LOCAL, Vec3D( 10 ,0 , 0 ) ); break;
		//case 'D': mCFCamera->translate( CFly::CFTO_LOCAL , Vec3D( -10 ,0 , 0 ) ); break;

		//case 'W': mCamControl->moveForward(); break;
		//case 'S': mCamControl->moveBack(); break;
		//case 'A': mCamControl->moveLeft(); break;
		//case 'D': mCamControl->moveRight(); break;
		//case 'O': mBloodBarComp->setLife( mBloodBarComp->getLife() - 10 ); break;
		//case 'P': mBloodBarComp->setLife( mBloodBarComp->getLife() + 10 ); break;
		case 'L': mPlayer->attack(); break;
		case 'M': mActorModel->changeAction( ANIM_ATTACK , true ); break;
		case 'N': mActorModel->changeAction( ANIM_WAIT , true ); break;
		case 'K': if ( mCDMask ) mCDMask->restore( 2 ); break;
		case 'R':
			createBoxEntity( Vec3D(200,200,200) );
			break;

		}

		return true;
	}

private:
	
	PlayerControl*       mPlayerControl;
	CameraView           mCamera;
	FristViewCamControl* mCamControl;

	CSceneLevel*         mSceneLevel;

	ISpatialComponent* mSpatialComp;
	CActorModel*       mActorModel;
	ActorMovement*     mMovement;
	CHero*             mPlayer;

	CEntity       mTestEntity;
	PhysicsWorld* mPhyWorld;

	CFObject*            mBoxObj;

	Viewport* mViewport;
	CFCamera* mCFCamera;
	CFScene*  mCFScene;
	CFWorld*  mCFWorld;
};

RPGProjectGame game;



int main( int argc , char* argv[] )
{    
	game.setUpdateTime( 1000 / 30 );
	game.run();
}
