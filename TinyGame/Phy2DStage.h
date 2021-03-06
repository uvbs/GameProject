#ifndef Phy2DStage_h__
#define Phy2DStage_h__

#include "Phy2D/Phy2D.h"

#include "StageBase.h"
#include "Coroutine.h"

namespace Phy2D
{
	FunctionJumper gDebugJumper;
	bool gDebugStep = true;

	void jumpDebug()
	{
		if ( gDebugStep )
			gDebugJumper.jump();
	}

	class Phy2DStageBase : public StageBase
	{
		typedef StageBase BaseClass;
	public:

		virtual bool onInit()
		{
			if ( !BaseClass::onInit() )
				return false;
			::Global::getGUI().cleanupWidget();
			if ( !::Global::getDrawEngine()->startOpenGL( true ) )
				return false;
			return true;
		}


		virtual void onEnd()
		{
			::Global::getDrawEngine()->stopOpenGL();
			BaseClass::onEnd();
		}

		virtual void onUpdate( long time )
		{
			BaseClass::onUpdate( time );

			int frame = time / gDefaultTickTime;
			for( int i = 0 ; i < frame ; ++i )
				tick();

			updateFrame( frame );
		}

		virtual void tick(){}
		virtual void updateFrame( int frame ){}

		void renderObject( GLGraphics2D& g , CollideObject& obj )
		{
			Shape* shape = obj.mShape;

			glPushMatrix();

			glTranslatef( obj.getPos().x , obj.getPos().y , 0 );
			glRotatef( Math::Rad2Deg( obj.mXForm.getRotateAngle() ) , 0 , 0 , 1 );
			glTranslatef( -obj.getPos().x , -obj.getPos().y , 0);

			switch( shape->getType() )
			{
			case Shape::eBox:
				{
					Vec2f ext = static_cast< BoxShape* >( shape )->mHalfExt;
					g.drawRect( obj.getPos() - ext , Vec2i( 2 * ext ) );
				}
				break;
			case Shape::eCircle:
				g.drawCircle( obj.getPos() , static_cast< CircleShape* >( shape )->getRadius() );
				break;
			case Shape::ePolygon:
				{
					PolygonShape* poly = static_cast< PolygonShape* >( shape );
					glTranslatef( obj.getPos().x , obj.getPos().y , 0 );
					g.drawPolygon( &poly->mVertices[0] , poly->mVertices.size() );
				}
				break;
			}
			glPopMatrix();
		}


	};
	class CollideTestStage : public Phy2DStageBase
	{
		typedef Phy2DStageBase BaseClass;
	public:
		CollideTestStage(){}

		virtual bool onInit()
		{
			if ( !BaseClass::onInit() )
				return false;

			mShapes[0] = &mShape1;
			mShapes[1] = &mShape2;
			mShapes[2] = &mShape3;
			mShape3.mVertices.push_back( Vec2f( 2 , 0  ) );
			mShape3.mVertices.push_back( Vec2f( 1 , 3 ) );
			mShape3.mVertices.push_back( Vec2f( -2 , 0 ) );
			mShape3.mVertices.push_back( Vec2f( -1 , -2 ) );
			mShape1.mHalfExt = Vec2f(1,1);
			mShape2.setRadius( 1 );
			mObjects[0].mShape = &mShape1;
			mObjects[0].mXForm.setTranslation( Vec2f( 0 , 0 ) );
			mObjects[1].mShape = &mShape2;
			//mObjects[1].mXForm.setRoatation( Math::Deg2Rad( 45 ) );
			mObjects[1].mXForm.setTranslation( Vec2f( 1 , 1 ) );


			restart();
			return true;
		}

		virtual void onEnd()
		{
			BaseClass::onEnd();
		}

		void onRender( float dFrame )
		{
			GLGraphics2D& g = Global::getGLGraphics2D();

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			g.beginRender();

			float scale = 50;
			Vec2f offset = Vec2f( 300 , 300 );

			glPushMatrix();
			glTranslatef( offset.x , offset.y , 0 );
			glScalef( scale , scale , scale );


			RenderUtility::setPen( g , Color::eRed );
			g.drawLine( Vec2f(-100,0) , Vec2f(100,0) );
			RenderUtility::setPen( g , Color::eGreen );
			g.drawLine( Vec2f(0,-100) , Vec2f(0,100) );


			RenderUtility::setPen( g , ( mIsCollided ) ? Color::eWhite : Color::eGray );
			RenderUtility::setBrush( g , Color::eNull );
			for( int i = 0 ; i < 2 ; ++i )
			{
				CollideObject& obj = mObjects[i];
				renderObject( g , obj );

			}
			FixString< 256 > str;

			//if ( mIsCollided )
			{
				RenderUtility::setPen( g , Color::eGreen );
				g.drawLine( mContact.pos[0] , mObjects[0].getPos() );
				g.drawLine( mContact.pos[1] , mObjects[1].getPos() );
				RenderUtility::setPen( g , Color::eYellow );
				g.drawLine( mObjects[0].getPos() + mContact.normal * mContact.depth , mObjects[0].getPos() );
				RenderUtility::setPen( g , Color::ePurple );
				g.drawLine( mObjects[0].getPos() - mContact.normal , mObjects[0].getPos() );


				XForm const& worldTrans = mObjects[0].mXForm;

				glPushMatrix();
				glTranslatef( worldTrans.getPos().x , worldTrans.getPos().y , 0 );
				glRotatef( Math::Rad2Deg( worldTrans.getRotateAngle() ) , 0 , 0 , 1 );

#if PHY2D_DEBUG	

				RenderUtility::setPen( g , Color::eOrange );
				for( int n = 0 ; n < 3 ; ++n )
				{
					Vec2f size = Vec2f(0.1,0.1);
					g.drawRect( gGJK.mSv[n]->vObj[0] - size / 2 , size );
				}
#endif //PHY2D_DEBUG
#if 0
				RenderUtility::setPen( g , Color::eBlue );
				for( int n = 0 ; n < 3 ; ++n )
				{
					Vec2f size = Vec2f(0.1,0.1);
					g.drawRect( gGJK.mSv[n]->v - size / 2 , size );
				}
#endif
				for( int n = 0 ; n < gGJK.mNumEdge ; ++n )
				{
					GJK::Edge& edge = gGJK.mEdges[n];
					Vec2f size = Vec2f(0.05,0.05);
					RenderUtility::setPen( g , Color::eCyan );
					g.drawRect( edge.sv->v - size / 2 , size );
					RenderUtility::setPen( g , Color::eCyan );
					g.drawLine( edge.sv->v , edge.sv->v + normalize( edge.sv->d ) );
				}
#if PHY2D_DEBUG	
				for( int n = 0 ; n < gGJK.mDBG.size() ; ++n )
				{
					GJK::Simplex& sv = gGJK.mDBG[n];
					Vec2f size = Vec2f(0.05,0.05);
					RenderUtility::setPen( g , Color::eGreen );
					g.drawRect( sv.v - size / 2 , size );
					RenderUtility::setPen( g , Color::eGreen );
					g.drawLine( sv.v , sv.v + 0.5 * normalize( sv.d ) );
				}
#endif //PHY2D_DEBUG

				glPopMatrix();
			}

			glPopMatrix();

			if ( mIsCollided )
			{
				g.drawText( 10 , 20 , str.format( "%f , %f , %f" , mContact.normal.x , mContact.normal.y , mContact.depth ) );
			}
			g.endRender();
		}



		void restart()
		{

		}


		virtual void tick()
		{
			//mObjects[0].mXForm.rotate( 0.0051 );
			//mIsCollided = mCollision.test( &mObjects[0] , &mObjects[1] , mContact );
		}

		virtual void updateFrame( int frame )
		{

		}

		bool onMouse( MouseMsg const& msg )
		{
			if ( !BaseClass::onMouse( msg ) )
				return false;
			return true;
		}
		void moveObject( Vec2f const& offset )
		{
			mObjects[1].mXForm.translate( offset );
			mIsCollided = mCollision.test( &mObjects[0] , &mObjects[1] , mContact );
		}

		bool onKey( unsigned key , bool isDown )
		{
			if ( !isDown )
				return false;

			static int idx = 0;
			float speed = 0.013;
			switch( key )
			{
			case 'R': restart(); break;
			case 'D': moveObject( Vec2f(speed,0) ); break;
			case 'A': moveObject( Vec2f(-speed,0) ); break; 
			case 'W': moveObject( Vec2f(0,speed) ); break; 
			case 'S': moveObject( Vec2f(0,-speed) ); break;
			case Keyboard::eLEFT:
				mObjects[1].mXForm.rotate( 0.01 ); break;
			case Keyboard::eRIGHT:
				mObjects[1].mXForm.rotate( -0.01 ); break;
			case Keyboard::eUP:
				++idx; if ( idx >= ARRAY_SIZE( mShapes ) ) idx = 0;
				mObjects[0].mShape = mShapes[idx]; 
				break;
			case Keyboard::eDOWN:
				--idx; if ( idx < 0 ) idx = ARRAY_SIZE( mShapes ) - 1;
				mObjects[0].mShape = mShapes[idx]; 
				break;
			case 'E':
				mIsCollided = mCollision.test( &mObjects[0] , &mObjects[1] , mContact );
				break;
			}
			return false;
		}
	protected:
		Contact mContact;

		bool  mIsCollided;
		CollideObject mObjects[2];
		BoxShape     mShape1;
		CircleShape  mShape2;
		PolygonShape mShape3;
		Shape*       mShapes[3];
		CollisionManager mCollision;
	};


	class WorldTestStage : public Phy2DStageBase
	{
		typedef Phy2DStageBase BaseClass;
	public:
		WorldTestStage(){}

		virtual bool onInit()
		{
			if ( !BaseClass::onInit() )
				return false;

			mShape3.mVertices.push_back( Vec2f( 2 , 0  ) );
			mShape3.mVertices.push_back( Vec2f( 1 , 3 ) );
			mShape3.mVertices.push_back( Vec2f( -2 , 0 ) );
			mShape3.mVertices.push_back( Vec2f( -1 , -2 ) );
			mBoxShape[0].mHalfExt = Vec2f(20,5);
			mBoxShape[1].mHalfExt = Vec2f(5,20);
			mBoxShape2.mHalfExt = Vec2f(4,4);
			mCircleShape.setRadius( 1 );

			BodyInfo info;
			RigidBody* body;
			body = mWorld.createRigidBody( &mBoxShape[0] , info );
			body->setMotionType( BodyMotion::eStatic );
			body->setPos( Vec2f( 0 , 0 ) );

			//body = mWorld.createRigidBody( &mBoxShape[0] , info );
			//body->setMotionType( BodyMotion::eStatic );
			//body->setPos( Vec2f( 0 , 40 ) );

			//body = mWorld.createRigidBody( &mBoxShape[1] , info );
			//body->setMotionType( BodyMotion::eStatic );
			//body->setPos( Vec2f( -10 , 20 ) );

			//body = mWorld.createRigidBody( &mBoxShape[1] , info );
			//body->setMotionType( BodyMotion::eStatic );
			//body->setPos( Vec2f( 10 , 20 ) );

		
			body = mWorld.createRigidBody( &mCircleShape , info );
			body->setPos( Vec2f( 0 , 6 ) );
			mBody[0] = body;

			body = mWorld.createRigidBody( &mCircleShape , info );
			body->setPos( Vec2f( 0 , 8 ) );
			mBody[1] = body;

			

			//body = mWorld.createRigidBody( &mCircleShape , info );
			//body->setPos( Vec2f( 0 , 20 ) );


			std::function< void () > fun = std::bind( &WorldTestStage::debugEntry , this );
			gDebugJumper.start( fun );

			restart();
			return true;
		}

		void debugEntry()
		{
			jumpDebug();
			while( gDebugStep )
			{
				mWorld.simulate( gDefaultTickTime / 1000.0f );
			}
		}

		virtual void onEnd()
		{
			BaseClass::onEnd();
		}


		void onRender( float dFrame )
		{
			GLGraphics2D& g = Global::getGLGraphics2D();

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			g.beginRender();

			float scale = 10;
			Vec2f offset = Vec2f( 300 , 500 );

			glPushMatrix();
			glTranslatef( offset.x , offset.y , 0 );
			glScalef( scale , -scale , scale );


			RenderUtility::setPen( g , Color::eRed );
			g.drawLine( Vec2f(-100,0) , Vec2f(100,0) );
			RenderUtility::setPen( g , Color::eGreen );
			g.drawLine( Vec2f(0,-100) , Vec2f(0,100) );


			RenderUtility::setBrush( g , Color::eNull );
			for( World::RigidBodyList::iterator iter = mWorld.mRigidBodies.begin() , itEnd = mWorld.mRigidBodies.end();
				 iter != itEnd ; ++iter )
			{
				RigidBody* body = *iter;
				RenderUtility::setPen( g , Color::eWhite );
				renderObject( g , *body );
				CollisionProxy* proxy = body->mProxy;

				RenderUtility::setPen( g , Color::eYellow );
				g.drawRect( proxy->aabb.min , proxy->aabb.max - proxy->aabb.min );

				RenderUtility::setPen( g , Color::eRed );
				Vec2f const xDir = body->mXForm.getRotation().getXDir();
				Vec2f const yDir = body->mXForm.getRotation().getYDir();
				RenderUtility::setPen( g , Color::eRed );
				g.drawLine( body->getPos() , body->getPos() + 0.5 * xDir );
				RenderUtility::setPen( g , Color::eGreen );
				g.drawLine( body->getPos() , body->getPos() + 0.5 * yDir );

				RenderUtility::setPen( g , Color::eOrange );
				g.drawLine( body->getPos() , body->getPos() + body->mLinearVel );

			}

			FixString< 256 > str;

			glPopMatrix();

			if ( !mWorld.mColManager.mMainifolds.empty() )
			{
				ContactManifold& cm = *mWorld.mColManager.mMainifolds[0];
				Contact& c = cm.mContect;

				RigidBody* bodyA = static_cast< RigidBody* >( c.object[0] );
				RigidBody* bodyB = static_cast< RigidBody* >( c.object[1] );

				Vec2f cp = 0.5 * ( c.pos[0] + c.pos[1] );

				Vec2f vA = bodyA->getVelFromWorldPos( cp );
				Vec2f vB = bodyB->getVelFromWorldPos( cp );
				Vec2f rA = cp - bodyA->mPosCenter;
				Vec2f rB = cp - bodyB->mPosCenter;
				Vec2f vrel = vB - vA;
				float vn = vrel.dot( c.normal );

				Vec2f cpA = bodyA->mXForm.mul( c.posLocal[0] );
				Vec2f cpB = bodyB->mXForm.mul( c.posLocal[1] );

				//TODO: normal change need concerned
				float depth2 = c.normal.dot( cpA - cpB );

				g.drawText( 100 , 20 , str.format( "vn = %f depth = %f" , vn , depth2 ) );
			}

			g.drawText( 100 , 30 , str.format( "v = %f %f" , mBody[0]->mLinearVel.x , mBody[0]->mLinearVel.y ) );
			g.drawText( 100 , 40 , str.format( "v = %f %f" , mBody[1]->mLinearVel.x , mBody[1]->mLinearVel.y ) );


				//g.drawText( 10 , 10 , str.format( "%f , %f , %f" , mContact.normal.x , mContact.normal.y , mContact.depth ) );


			g.endRender();
		}



		void restart()
		{

		}


		virtual void tick()
		{
			if ( !gDebugStep )
				mWorld.simulate( gDefaultTickTime / 1000.0f );
	
		}

		virtual void updateFrame( int frame )
		{

		}

		bool onMouse( MouseMsg const& msg )
		{
			if ( !BaseClass::onMouse( msg ) )
				return false;
			return true;
		}


		bool onKey( unsigned key , bool isDown )
		{
			if ( !isDown )
				return false;

			float speed = 0.013;
			switch( key )
			{
			case 'R': restart(); break;
			case 'D':  break;
			case 'A': break; 
			case 'W': break; 
			case 'S': break;
			case 'E': 	
				{
					BodyInfo info;
					RigidBody* body = mWorld.createRigidBody( &mCircleShape , info );
					body->setPos( Vec2f( 0 , 30 ) );
				}
				break;
			case Keyboard::eF2:
				{
					if ( gDebugStep )
					{
						gDebugStep = false;
						gDebugJumper.jump();
					}
					else
					{
						gDebugStep = true;
						std::function< void () > fun = std::bind( &WorldTestStage::debugEntry , this );
						gDebugJumper.start( fun );
					}
				}
				break;
			case 'Q': jumpDebug(); break;
			}
			return false;
		}
	protected:

		RigidBody*   mBody[2];

		World        mWorld;
		BoxShape     mBoxShape[2];
		BoxShape     mBoxShape2;
		CircleShape  mCircleShape;
		PolygonShape mShape3;

	};

}


#endif // Phy2DStage_h__
