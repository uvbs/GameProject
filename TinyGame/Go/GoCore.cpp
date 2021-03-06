#include "TinyGamePCH.h"
#include "GoCore.h"

#include "SocketBuffer.h"

#include <cassert>
#include <iostream>
#include <fstream>


namespace Go {

	char const EDGE_MARK    = -1;
	char const VISITED_MARK = 0x10; 


	static int const gDirOffset[4][2] = 
	{
		1,0,  -1,0,  0,-1,  0,1,
	};

	Board::Board() 
		:mData( NULL )
		,mLinkIndex( NULL )
		,mSize(0)
	{
	}

	Board::~Board()
	{
		delete [] mData;
		delete [] mLinkIndex;
	}

	void Board::setup( int size )
	{
		int capity = ( size + 1 ) * ( size + 2 );

		if ( mSize < size )
		{
			delete [] mData;
			delete [] mLinkIndex;

			mData      = new char[ capity ];
			mLinkIndex = new LinkType[ capity ];
			mSize     = size;
		}

		clear();

		mIndexOffset[eLeft  ] = -1;
		mIndexOffset[eRight ] = 1;
		mIndexOffset[eTop   ] = -getDataSizeX();
		mIndexOffset[eBottom] = getDataSizeX();
	}

	void Board::clear()
	{
		int capity = getDataSize();

		std::fill_n( mData , capity , (char)eEmpty );

		for ( int i = getDataSizeX() - 1 ; i < capity ; i += getDataSizeX() )
		{
			mData[i] = EDGE_MARK;
		}

		int offsetTop = capity - getDataSizeX();
		for ( int i = 0 ; i < getDataSizeX() ; ++i  )
		{
			mData[ i ] = EDGE_MARK;
			mData[ i + offsetTop ] = EDGE_MARK;
		}

		std::fill_n( mLinkIndex , capity , 0 );
	}

	int Board::getLife( int x , int y ) const
	{
		assert( getData( x , y ) != eEmpty );
		return getLife( getDataIndex( x, y ) );
	}

	int Board::getLife( int index ) const
	{
		assert( getData( index ) != eEmpty );
		int idxRoot = getLinkRoot( index );
		return -mLinkIndex[ idxRoot ];
	}

	void Board::putStone( int idx, DataType color )
	{
		assert( color != eEmpty ); 
		assert( mData[idx] == eEmpty );
		mData[ idx ] = color;

		mIdxConRoot = idx;
		mLinkIndex[ idx ] = 0;

		for( int i = 0 ; i < NumDir ; ++i )
		{
			int idxCon = calcConIndex( idx , i );
			char data = getData( idxCon );

			if ( data == EDGE_MARK )
				continue;

			if ( data == eEmpty )
			{
				addRootLife( mIdxConRoot , 1 );
			}
			else
			{
				int idxRoot = getLinkRoot( idxCon );
				if ( data == color && mIdxConRoot != idxRoot )
				{
					linkRoot( idxRoot );
				}
				else
				{
					addRootLife( idxRoot , -1 );
				}
			}
		}
	}

	int Board::fillStone( Pos const& p , DataType color )
	{
		mColorR = color;
		assert( color != eEmpty );
		int idx = p.toIndex();
		return fillStone_R( idx );
	}

	int Board::fillStone_R( int idx )
	{
		char data = getData( idx );
		if ( data != eEmpty )
			return 0;

		putStone( idx , mColorR );

		int result = 1;
		for( int i = 0 ; i < NumDir ; ++i )
		{
			int idxCon = calcConIndex( idx , i );
			result += fillStone_R( idxCon );
		}

		return result;
	}

	void Board::linkRoot( int idxRoot )
	{
		assert( idxRoot != mIdxConRoot );

		int life1 = mLinkIndex[ mIdxConRoot ];
		int life2 = mLinkIndex[ idxRoot ];

		int totalLive = life1 + life2 + 1;
		if ( life1 > life2 )
		{
			mLinkIndex[ mIdxConRoot ] = idxRoot;
			mLinkIndex[ idxRoot ] = totalLive;
			mIdxConRoot = idxRoot;
		}
		else
		{
			mLinkIndex[ idxRoot ] = mIdxConRoot;
			mLinkIndex[ mIdxConRoot ] = totalLive;
		}
	}

	void Board::removeStone( Pos const& p )
	{
		int  idx = p.toIndex();
		char data = getData( idx );
		assert( data != eEmpty );

		mData[ idx ] = eEmpty;

		for( int dir = 0 ; dir < NumDir ; ++dir )
		{
			int idxCon = calcConIndex( idx , dir );
			char dataCon = mData[ idxCon ];
			if ( dataCon == EDGE_MARK || dataCon == eEmpty )
				continue;

			if ( dataCon & VISITED_MARK )
				continue;

			if ( dataCon != data )
			{
				addRootLife( getLinkRoot( idxCon ) , 1 );
			}
			else
			{
				relink( idxCon );
			}
		}

		for( int dir = 0 ; dir < NumDir ; ++dir )
		{
			int idxCon = calcConIndex( idx , dir );
			if ( mData[ idxCon ] & VISITED_MARK )
				removeVisitedMark_R( idxCon );
		}
	}

	Board::DataType Board::getData( int x , int y ) const 
	{
		int idx = getDataIndex( x , y );
		assert( getData( idx ) != EDGE_MARK );
		return DataType( getData( idx ) );
	}

	bool Board::checkRange( int x , int y ) const
	{
		return 0 <= x && x < mSize && 0 <= y && y < mSize;
	}

	int Board::getLinkRoot( int idx ) const
	{
		while( mLinkIndex[idx] > 0 )
		{
			idx = mLinkIndex[idx];
		}
		return idx;
	}

	void Board::addRootLife( int idxRoot , int val )
	{
		assert( getLinkRoot( idxRoot ) == idxRoot );
		mLinkIndex[ idxRoot ] -= val;
		assert( mLinkIndex[ idxRoot ] <= 0 );
	}

	int Board::captureStone_R( int idx )
	{
		char data = getData( idx );
		if ( data == EDGE_MARK || data == eEmpty )
			return 0;

		if ( data != mColorR )
		{
			int idxRoot = getLinkRoot( idx );
			addRootLife( idxRoot , 1 );
			return 0;
		}

		mData[ idx ] = eEmpty;

		int result = 1;
		for( int dir = 0 ; dir < NumDir ; ++dir )
			result += captureStone_R( calcConIndex( idx , dir ) );

		return result;
	}

	int Board::captureStone( Pos const& p )
	{
		mColorR = getData( p );
		assert( mColorR != eEmpty );
		return captureStone_R( p.toIndex() );
	}

	int Board::relink( int idx )
	{
		char data = getData( idx );
		assert( data != eEmpty && data != EDGE_MARK );

		mData[ idx ] |= VISITED_MARK;
		mIdxConRoot = idx;
		mColorR = DataType( data );

		int life = 0;
		for( int dir = 0 ; dir < NumDir ; ++dir )
		{
			life += relink_R( calcConIndex( idx , dir ) );
		}
		mLinkIndex[ idx ] = -life;
		return life;
	}

	int Board::relink_R( int idx )
	{
		char data = mData[ idx ];

		if ( data == eEmpty )
			return 1;
		if ( data == EDGE_MARK )
			return 0;
		if ( data & VISITED_MARK )
			return 0;
		if ( data != mColorR )
			return 0;

		mData[ idx ] |= VISITED_MARK;
		mLinkIndex[ idx ] = mIdxConRoot;

		int life = 0;
		for( int dir = 0 ; dir < NumDir ; ++dir )
			life += relink_R( calcConIndex( idx , dir ) );

		return life;
	}

	void Board::removeVisitedMark_R( int idx )
	{
		if ( mData[ idx ] == EDGE_MARK )
			return;

		if ( mData[ idx ] & VISITED_MARK )
		{
			mData[ idx ] &= ~VISITED_MARK;
			for( int dir = 0 ; dir < NumDir ; ++dir )
				removeVisitedMark_R( calcConIndex( idx , dir ) );
		}
	}

	bool Board::getConPos( Pos const& pos , int dir , Pos& result )
	{
		int idx = calcConIndex( pos.index , dir );
		
		if ( getData( idx ) == EDGE_MARK )
			return false;

		result.index = idx;
		return true;
	}

	Game::Game()
	{

	}

	void Game::setup( int size )
	{
		mBoard.setup( size );
		doRestart( false );
	}

	void Game::restart()
	{
		doRestart( true );
	}

	void Game::doRestart( bool beClearBoard )
	{
		if ( beClearBoard )
			mBoard.clear();
		mCurHand  = 0;
		mNextPlayColor = Board::eBlack;
		mNumBlackRemoved = 0;
		mNumWhiteRemoved = 0;
		mIdxKoPos    = -1;
		mStepVec.clear();
	}

	bool Game::play( int x , int y )
	{
		if ( !mBoard.checkRange( x , y ) )
			return false;

		Board::Pos pos = mBoard.getPos( x , y );

		return play(pos);

	}

	bool Game::play( Pos const & pos )
	{
		if ( mBoard.getData( pos ) != Board::eEmpty )
			return false;

		mBoard.putStone( pos , mNextPlayColor );

		unsigned bitDir = 0;
		int numCapture = captureStone( pos , bitDir );

		if ( numCapture == 0 )
		{
			if ( mBoard.getLife( pos ) == 0 )
			{
				mBoard.removeStone( pos );
				return false;
			}
		}
		else if ( numCapture == 1 )
		{
			if ( pos.toIndex() == mIdxKoPos )
			{
				Pos cPos = getFristConPos( pos , bitDir );

				mBoard.fillStone( cPos , mNextPlayColor == Board::eBlack ? Board::eWhite : Board::eBlack );
				mBoard.removeStone( pos );
				return false;
			}
		}

		StepInfo info;
		info.bitCaptureDir  = bitDir;
		info.idxPlay        = pos.toIndex();
		info.idxKO          = mIdxKoPos;
		mStepVec.push_back( info );


		if ( numCapture == 1 )
		{
			Pos cPos = getFristConPos( pos , bitDir );
			mIdxKoPos = cPos.toIndex();
		}
		else
		{
			mIdxKoPos = -1;
		}

		++mCurHand;
		if ( mNextPlayColor == Board::eBlack )
		{
			mNumWhiteRemoved += numCapture;
			mNextPlayColor = Board::eWhite;
		}
		else
		{
			mNumBlackRemoved += numCapture;
			mNextPlayColor = Board::eBlack;
		}

		return true;
	}

	int Game::captureStone( Board::Pos const& pos , unsigned& bitDir )
	{
		DataType data = mBoard.getData( pos );

		int numCapture = 0;
		for( int dir = 0 ; dir < Board::NumDir ; ++dir )
		{
			Pos posCon;
			if ( !mBoard.getConPos( pos , dir , posCon ) )
				continue;

			char dataCon = mBoard.getData( posCon );

			if ( dataCon == Board::eEmpty || dataCon == data )
				continue;

			if ( mBoard.getLife( posCon ) != 0 )
				continue;

			numCapture += mBoard.captureStone( posCon );
			bitDir |= ( 1 << dir );
		}

		return numCapture;
	}

	Game::Pos Game::getFristConPos( Board::Pos const& pos , unsigned bitDir )
	{
		assert( bitDir & 0x0f );

		for( int dir = 0 ; dir < Board::NumDir ; ++dir )
		{
			if ( bitDir & ( 1 << dir ) )
			{
				Pos result;
				bool isOK = mBoard.getConPos( pos , dir , result );
				assert( isOK );
				return result;
			}
		}
		assert( 0 );
		return Pos( -1 );
	}

	void Game::pass()
	{
		StepInfo info;
		info.idxPlay   = -1;
		info.bitCaptureDir = 0;
		info.idxKO = mIdxKoPos;
	}

	void Game::undo()
	{
		if ( mStepVec.empty() )
			return;

		StepInfo& step = mStepVec.back();

		DataType color = mNextPlayColor == Board::eBlack ? Board::eWhite : Board::eBlack;

		if ( step.idxPlay != -1 )
		{
			Board::Pos posRemove = mBoard.getPos( step.idxPlay );

			if ( step.bitCaptureDir )
			{
				int num = 0;
				for( int dir = 0 ; dir < Board::NumDir ; ++dir )
				{
					if ( step.bitCaptureDir & ( 1 << dir ) )
					{
						Board::Pos p;
						mBoard.getConPos( posRemove , dir , p );
						num += mBoard.fillStone( p , mNextPlayColor );
					}
				}

				if ( color == Board::eBlack )
				{
					mNumWhiteRemoved -= num;
				}
				else
				{
					mNumBlackRemoved -= num;
				}
			}

			mBoard.removeStone( posRemove );
		}

		--mCurHand;
		mIdxKoPos = step.idxKO;
		mNextPlayColor = color;
		mStepVec.pop_back();
	}

	void Game::print( int x , int y )
	{
		using namespace std;
		static char const* dstr[] = 
		{
			"┌","┬","┐",
			"├","┼","┤",
			"└","┴","┘",
		};

		int size = mBoard.getSize();

		for( int j = 0 ; j < size ; ++ j )
		{
			for( int i = 0 ; i < size ; ++ i )
			{
				if ( i == x && j == y )
				{
					cout << "⊕";
					continue;
				}

				switch ( mBoard.getData( i , j ) )
				{
				case Board::eBlack: cout << "○" ; break;
				case Board::eWhite: cout << "●" ; break;
				case Board::eEmpty:
					{
						int index = 0;
						if ( i != 0 )
						{ 
							index += ( i != ( size - 1 ) ) ? 1 : 2;
						}
						if ( j != 0 )
						{
							index += 3 * ( ( j != ( size - 1 ) ) ? 1 : 2 );
						}
						cout << dstr[ index ];
					}
					break;
				}
			}
			cout << endl;
		}

		for ( int dir = 0 ; dir < Board::NumDir ; ++dir )
		{
			int nx = x + gDirOffset[dir][0];
			int ny = y + gDirOffset[dir][1];

			if ( !mBoard.checkRange( nx , ny ) )
				continue;
			DataType nType = mBoard.getData( nx , ny );
			if ( nType == Board::eEmpty )
				continue;
			int life = mBoard.getLife( nx , ny ) ;
			std::cout << "dir =" << dir << " life = "<< life << std::endl;
		}
	}

	void Game::takeData( SBuffer& buffer )
	{
		unsigned size = mStepVec.size();
		buffer.fill( size );

		for( int i = 0 ; i < mStepVec.size() ; ++i )
		{
			StepInfo& info = mStepVec[i];
			buffer.fill( info.idxPlay );
		}
	}

	bool Game::restoreData( SBuffer& buffer )
	{
		mStepVec.clear();

		restart();

		unsigned size;
		buffer.take( size );

		for( int i = 0 ; i < size ; ++i )
		{
			int idxPos;
			buffer.take( idxPos );
			if ( idxPos == -1 )
			{
				pass();
			}
			else if ( play( mBoard.getPos( idxPos ) ) )
			{
				return false;
			}
		}
		return true;
	}

	bool Game::save( char const* path )
	{
		std::ofstream fs( path );
		if ( !fs.is_open() )
			return false;

		fs << '(' << ';';
		fs << "SZ" << mBoard.getSize();
		bool beBlack = true;
		for( StepVec::iterator iter = mStepVec.begin(); 
			 iter != mStepVec.end() ; ++iter )
		{
			StepInfo& step = *iter;
			if ( step.idxPlay != -1 )
			{
				fs << ';' << ( ( beBlack ) ? 'B' : 'W' ) << '[';
			}
			beBlack = !beBlack;
		}

		return true;
	}

}//namespace Go