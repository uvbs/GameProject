#ifndef AStarStage_h__
#define AStarStage_h__

#include "StageBase.h"
#include "AStar.h"
#include "AStarTile2D.h"
#include "TVector2.h"

#include <algorithm>

namespace AStar
{
	typedef TVector2< int > Vec2i;

	class PathGraph
	{




	};

	class PathFinder
	{
	public:

		std::vector< PathGraph* > mPathGraphs;
	};

	
	static Vec2i const gNeighborOffset[] = { Vec2i(1,0) , Vec2i( -1 , 0 ) , Vec2i( 0 , 1 ) , Vec2i( 0, -1 ) };
	static uint8 const gTestMask[] = { (BIT(0)|BIT(2)) , (BIT(1)|BIT(2)) ,(BIT(1)|BIT(3)) ,(BIT(0)|BIT(3)) };
	static Vec2i const gNeighborOffset2[] = { Vec2i(1,1) , Vec2i( -1 , 1 ) , Vec2i( -1 , -1 ) , Vec2i( 1 , -1 ) };

	class MyAStar : public AStarTile2DT< MyAStar >
	{
	public:
		MyAStar()
		{
			mClearance = 1;
		}
		ScoreType calcHeuristic( StateType& state )
		{  
			return calcDistance( state , mGoalPos );
		}
		ScoreType calcDistance( StateType& a, StateType& b )
		{  
			return 1000 * ( abs( a.x - b.x ) + abs( a.y - b.y ) );
		}
		bool      isGoal (StateType& state )
		{ 
			if ( mClearance == 1 )
				return mGoalPos == state;

			Vec2i dif = mGoalPos - state;
			return 0 <= dif.x && dif.x < mClearance &&
				   0 <= dif.y && dif.y < mClearance;
		}
		//  call addSreachNode for all possible next state
		void      processNeighborNode( NodeType& node )
		{
			
			uint8 mask = 0;
			for( int i = 0 ; i < 4 ; ++i )
			{
				Vec2i newPos = node.state + gNeighborOffset[i];

				if ( !mMap.checkRange( newPos.x , newPos.y ) )
					continue;

				if ( node.parent && newPos == node.parent->state )
					continue;

				Tile& tile = mMap.getData( newPos.x , newPos.y );
				if ( tile.clearance < mClearance )
					continue;

				mask |= BIT( i );
				addSreachNode( newPos , node , 1000 );
			}


			for( int i = 0 ; i < 4 ; ++i )
			{
				if ( ( mask & gTestMask[i] ) != gTestMask[i] )
					continue;

				Vec2i newPos = node.state + gNeighborOffset2[i];

				Tile& tile = mMap.getData( newPos.x , newPos.y );
				if ( tile.clearance < mClearance )
					continue;

				addSreachNode( newPos , node , 1414 );
			}
		}

		struct Portal
		{
			Vec2i pos;
		};

		struct Edge
		{
			uint32  con[2];
			uint8   clearance;
			bool    beOuter;
		};

		struct Cell
		{
			std::vector< Portal > portals;
			std::vector< Edge >   links;
		};

		bool haveBlock( int x , int y )
		{
			return mMap( x , y ).terrain != 0;
		}

		void contructPortalH( Vec2i const& start , int length , Cell& cellA , Cell& cellB )
		{
			Vec2i posA = start;
			Vec2i posB = start + Vec2i(1,0);

			for( int offset = 0 ; offset < length; ++offset )
			{
				Tile& tileA = mMap.getData( posA.x , posA.y );
				Tile& tileB = mMap.getData( posB.x , posB.y );

				if ( tileA.clearance != 0 && tileB.clearance != 0 )
				{
					uint8 minClear = std::min( tileA.clearance , tileB.clearance );
					//add portal
					offset += minClear;
					posA.y += minClear;
					posB.y += minClear;
				}
				else
				{
					offset += 1;
					posA.y += 1;
					posB.y += 1;
				}	
			}
		}

		void contructPortalV( Vec2i const& start , int length , Cell& cellA , Cell& cellB )
		{
			Vec2i posA = start;
			Vec2i posB = start + Vec2i(0,1);

			for( int offset = 0 ; offset < length; ++offset )
			{
				Tile& tileA = mMap.getData( posA.x , posA.y );
				Tile& tileB = mMap.getData( posB.x , posB.y );

				if ( tileA.clearance != 0 && tileB.clearance != 0 )
				{
					uint8 minClear = std::min( tileA.clearance , tileB.clearance );

					Portal portal;
					portal.pos = posA;
					cellA.portals.push_back( portal );

					//add portal
					offset += minClear;
					posA.x += minClear;
					posB.x += minClear;
				}
				else
				{
					offset += 1;
					posA.x += 1;
					posB.x += 1;
				}	
			}
		}




		void inputMap( int w , int h , uint8* terrain )
		{
			getMap().resize( w , h );
			mMap.resize( w , h );

			for( int j = 0 ; j < mMap.getSizeY() ; ++j )
			for( int i = 0 ; i < mMap.getSizeX() ; ++i )
			{
				int idx = mMap.toIndex( i , j );
				mMap[ idx ].terrain = terrain[ idx ];
			}
			updateClearance();
		}
		void updateClearance()
		{
			int xMax = mMap.getSizeX() - 1;
			int yMax = mMap.getSizeY() - 1;
			
			for( int i = 0 ; i < mMap.getSizeX() ; ++i )
				mMap( i , yMax ).clearance = haveBlock( i , yMax ) ? 0 : 1;

			for ( int j = yMax - 1 ; j >= 0 ; --j )
			{
				uint8 prevVal = haveBlock( xMax , j ) ? 0 : 1;
				mMap( xMax , j ).clearance = prevVal;

				for ( int i = xMax - 1 ; i >= 0 ; --i )
				{
					prevVal = calcClearance( i , j , prevVal );
					mMap( i , j ).clearance = prevVal;
				}
			}
		}



		uint8 calcClearance( int x , int y , uint8 prevVal )
		{
			assert( mMap( x + 1 , y ).clearance == prevVal );

			if ( haveBlock( x , y ) )
				return 0;
			//   ____________________
            //   | out |   prev     |
			//   |_____|______      |  
			//   [     |      |     |
			//   | temp|______|_____|
			//   |            |	    |
            //   |____________|_____|
			uint8 temp = mMap( x , y + 1 ).clearance;
			if ( prevVal > temp  )
				return temp + 1;

			if ( prevVal < temp )
				return prevVal + 1;

			if ( prevVal == 0 )
				return 1;

			if ( mMap( x + prevVal , y + prevVal ).clearance )
				return prevVal + 1;

			return prevVal;
		}
		void updateClearance( int x , int y )
		{
			if ( x == mMap.getSizeX() - 1 )
			{
				mMap( x , y ).clearance = haveBlock( x , y ) ? 0 : 1;
				x -= 1;
				if ( y == mMap.getSizeY() - 1 )
					y -= 1;
			}
			else if ( y == mMap.getSizeY() - 1 )
			{
				mMap( x , y ).clearance = haveBlock( x , y ) ? 0 : 1;
				y -= 1;
			}

			int xMin = 0;
			for ( int j = y; j >= 0 ; --j )
			{
				for ( int i = x; i >= xMin ; --i )
				{


				}
			}
		}


		struct Tile
		{
			uint8 terrain;
			uint8 clearance;
		};

		static uint8 const MaxClearance = 8;
		typedef TGrid2D< Tile > TileMap;
		TileMap mMap;
		
		uint8   mClearance;
		Vec2i   mGoalPos;

	};

	uint8 map[] =
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	};

	class View
	{
	public:
		static int const TileLen = 24;
		static Vec2i SC_Offset(){ return Vec2i( 20 , 20 ); }
		Vec2i convertToScreen( Vec2i const& pos ){ return SC_Offset() + TileLen * pos; }
		Vec2i convertToWorld( Vec2i const& pos ) { return ( pos - SC_Offset() ) / TileLen; }
	};

	class TestStage : public StageBase
		            , public View
	{
		typedef StageBase BaseClass;
	public:
		TestStage()
		{

		}

		virtual bool onInit()
		{
			::Global::getGUI().cleanupWidget();
			restart();
			return true;
		}

		virtual void onUpdate( long time )
		{
			BaseClass::onUpdate( time );

			int frame = time / gDefaultTickTime;
			for( int i = 0 ; i < frame ; ++i )
				tick();

			updateFrame( frame );
		}

		void onRender( float dFrame )
		{
			Graphics2D& g = Global::getGraphics2D();

			MyAStar::TileMap& map = mAStar.mMap;

			RenderUtility::setPen( g , Color::eBlack );

			g.setTextColor( 125 , 125 , 0 );
			for( int i = 0 ; i < map.getSizeX(); ++i )
			for( int j = 0 ; j < map.getSizeY(); ++j )
			{
				MyAStar::Tile& tile = map.getData( i , j );

				Vec2i renderPos = convertToScreen( Vec2i( i , j ) );

				if ( tile.terrain )
				{
					RenderUtility::setBrush( g , Color::eGray );
				}
				else
				{
					RenderUtility::setBrush( g , Color::eWhite );
				}

				g.drawRect( renderPos , Vec2i( TileLen , TileLen ) );

				FixString< 32 > str;
				str.format( "%d" , tile.clearance );
				g.drawText( renderPos , Vec2i( TileLen , TileLen ) , str );
			}


			
			if ( mPath )
			{
				MyAStar::MapType& map = mAStar.getMap();

				

				for ( MyAStar::MapType::iterator iter = map.begin() , itEnd = map.end();
					iter != itEnd ; ++iter )
				{
					MyAStar::NodeType* node = *iter;

					if ( !node->parent )
						continue;

					if ( !node->isClose() )
					{
						RenderUtility::setPen( g , Color::eGreen );
						Vec2i p1 = convertToScreen( node->state ) + Vec2i( TileLen , TileLen ) / 2;
						Vec2i p2 = convertToScreen( node->parent->state ) + Vec2i( TileLen , TileLen ) / 2;
						g.drawLine( p1 , p2 );
					}
					else if ( !node->isPath() )
					{
						RenderUtility::setPen( g , Color::eBlue );
						Vec2i p1 = convertToScreen( node->state ) + Vec2i( TileLen , TileLen ) / 2;
						Vec2i p2 = convertToScreen( node->parent->state ) + Vec2i( TileLen , TileLen ) / 2;
						g.drawLine( p1 , p2 );
					}
				}

				RenderUtility::setPen( g , Color::eRed );

				MyAStar::NodeType* node = mPath;
				Vec2i prevPos = convertToScreen( mStartPos ) + Vec2i( TileLen , TileLen ) / 2;
				do
				{
					Vec2i pos = convertToScreen( node->state ) + Vec2i( TileLen , TileLen ) / 2;
					g.drawLine( pos , prevPos );
					node = node->child;
					prevPos = pos;
				}
				while( node );
			}
		}


		void restart()
		{
			mAStar.inputMap( 15 , 15 , map );
			mAStar.mClearance = 1;
			mStartPos = Vec2i( 0 ,0 );
			mPath = nullptr;
		}


		void tick()
		{

		}

		void updateFrame( int frame )
		{

		}

		bool onMouse( MouseMsg const& msg )
		{
			if ( !BaseClass::onMouse( msg ) )
				return false;

			Vec2i tilePos = convertToWorld( msg.getPos() );
			if ( mAStar.mMap.checkRange( tilePos.x , tilePos.y ) )
			{
				if ( msg.onLeftDown() )
				{
					mStartPos = tilePos;
				}
				else if ( msg.onRightDown() )
				{
					mPath = nullptr;
					mAStar.mGoalPos = tilePos;
					if ( mAStar.sreach( mStartPos ) )
					{
						mPath = mAStar.getPath();
					}
				}
			}

			return true;
		}

		bool onKey( unsigned key , bool isDown )
		{
			if ( !isDown )
				return false;

			switch( key )
			{
			case 'R': restart(); break;
			}
			return false;
		}
		MyAStar::NodeType* mPath;
		Vec2i   mStartPos;
		MyAStar mAStar;
	};



}//namespace AStar

#endif // AStarStage_h__
