#include "GLGraphics2D.h"

#include <cassert>
#include <algorithm>

static char const* vertexShader =
	"varying vec2 vTexCoord;"
	"void main(void)"
	"{"
		"vTexCoord = gl_MultiTexCoord0.xy;"
		"gl_Position = ftransform();"
	"}";
static char const* fragmentShader = 
	"sampler2D myTexture;"
	"varying vec2 vTexCoord;"
	"uniform vec3 colorKey;"
	"void main(void)"
	"{"
		"vec4 color = texture2D(myTexture, vTexCoord);"
		"if (color.rgb == colorKey )"
			"discard;"
		"gl_FragColor = color;"
	"}";

#define DRAW_LINE_IMPL( EXPR )\
	mBuffer.clear();\
	EXPR;\
	glEnableClientState( GL_VERTEX_ARRAY );\
	glVertexPointer( 2 , GL_FLOAT , 0 , &mBuffer[0] );\
	if ( mDrawPen )\
	{\
		glColor4ub( mColorPen.r , mColorPen.g , mColorPen.b , mAlpha );\
		glDrawArrays( GL_LINE_LOOP , 0 , mBuffer.size() / 2 );\
	}\
	glDisableClientState( GL_VERTEX_ARRAY );

#define DRAW_IMPL( EXPR )\
	mBuffer.clear();\
	EXPR;\
	glEnableClientState( GL_VERTEX_ARRAY );\
	glVertexPointer( 2 , GL_FLOAT , 0 , &mBuffer[0] );\
	if ( mDrawBrush )\
	{\
		glColor4ub( mColorBrush.r , mColorBrush.g , mColorBrush.b , mAlpha );\
		glDrawArrays( GL_POLYGON , 0 , mBuffer.size() / 2 );\
	}\
	if ( mDrawPen )\
	{\
		glColor4ub( mColorPen.r , mColorPen.g , mColorPen.b , mAlpha );\
		glDrawArrays( GL_LINE_LOOP , 0 , mBuffer.size() / 2 );\
	}\
	glDisableClientState( GL_VERTEX_ARRAY );

static inline int calcCircleSemgmentNum( int r )
{
	return std::max( 4 * ( r / 2 ) , 32 );
}


GLGraphics2D::GLGraphics2D()
	:mWidth(1)
	,mHeight(1)
{
	mFont = NULL;
	mWidthPen = 1;
	mDrawBrush = true;
	mDrawPen   = true;
	mAlpha    = 255;
	mColorKeyShader = 0;
	mColorBrush = ColorKey3( 0 , 0 , 0 );
	mColorPen = ColorKey3( 0 , 0 , 0 );
}

void GLGraphics2D::init( int w , int h )
{
	mWidth = w;
	mHeight = h;
}

void GLGraphics2D::beginXForm()
{

}

void GLGraphics2D::finishXForm()
{

}

void GLGraphics2D::beginRender()
{
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0 , mWidth , mHeight , 0 , -1 , 1 );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
}

void GLGraphics2D::endRender()
{
	glFlush();

	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}

void GLGraphics2D::emitPolygonVertex(Vec2f pos[] , int num)
{
	for( int i = 0 ; i < num ; ++i )
	{
		emitVertex( pos[i].x , pos[i].y );
	}
}

void GLGraphics2D::emitPolygonVertex(Vec2i pos[] , int num)
{
	for( int i = 0 ; i < num ; ++i )
	{
		emitVertex( pos[i].x , pos[i].y );
	}
}

void GLGraphics2D::emintRectVertex(Vec2f const& p1 , Vec2f const& p2)
{
	emitVertex( p1.x , p1.y );
	emitVertex( p2.x , p1.y );
	emitVertex( p2.x , p2.y );
	emitVertex( p1.x , p2.y );
}

void GLGraphics2D::emitCircleVertex(float cx, float cy, float r, int numSegment)
{
	float theta = 2 * 3.1415926 / float(numSegment); 
	float c = cos(theta);//precalculate the sine and cosine
	float s = sin(theta);

	float x = r;//we start at angle = 0 
	float y = 0;
	float v[2];
	for(int i = 0; i < numSegment; ++i) 
	{ 
		v[0] = cx + x;
		v[1] = cy + y;
		emitVertex( v );

		//apply the rotation matrix
		float temp = x;
		x = c * temp - s * y;
		y = s * temp + c * y;
	}
}

void GLGraphics2D::emitEllipseVertex(float cx, float cy, float r , float yFactor , int numSegment)
{
	float theta = 2 * 3.1415926 / float(numSegment); 
	float c = cos(theta);//precalculate the sine and cosine
	float s = sin(theta);

	float x = r;//we start at angle = 0 
	float y = 0;
	float v[2];
	for(int i = 0; i < numSegment; ++i) 
	{ 
		v[0] = cx + x;
		v[1] = cy + yFactor * y;
		emitVertex( v );
		float temp = x;
		x = c * temp - s * y;
		y = s * temp + c * y;
	}
}

void GLGraphics2D::emitRoundRectVertex( Vec2f const& pos , Vec2f const& rectSize , Vec2f const& circleSize)
{
	int numSegment = calcCircleSemgmentNum( ( circleSize.x + circleSize.y ) / 2 );
	float yFactor = float( circleSize.y ) / circleSize.x;
	int num = numSegment / 4;

	float theta = 2 * 3.1415926 / float(numSegment); 
	float c = cos(theta);//precalculate the sine and cosine
	float s = sin(theta);

	float cvn[4][2] =
	{
		float( pos.x + rectSize.x - circleSize.x ) , float( pos.y + rectSize.y - circleSize.y ) ,
		float( pos.x + circleSize.x ) , float( pos.y + rectSize.y - circleSize.y ) ,
		float( pos.x + circleSize.x ) , float( pos.y + circleSize.y ) ,
		float( pos.x + rectSize.x - circleSize.x ) , float( pos.y + circleSize.y ) ,
	};

	float v[2];
	float x , y;
	float cx = cvn[0][0];
	float cy = cvn[0][1];
	for( int n = 0 ; n < 4 ; ++n )
	{
		switch( n )
		{
		case 0: x = circleSize.x; y = 0; break;
		case 1: y = circleSize.x; x = 0; break;
		case 2: x = -circleSize.x; y = 0; break;
		case 3: y = -circleSize.x; x = 0; break;
		}
		for(int i = 0; i < num; ++i) 
		{ 
			v[0] = cx + x;
			v[1] = cy + yFactor * y;
			emitVertex( v );

			float temp = x;
			x = c * temp - s * y;
			y = s * temp + c * y;
		}
		int next = ( n == 3 ) ? 0 : n + 1;
		cx = cvn[next][0];
		cy = cvn[next][1];
		v[0] = cx + x;
		v[1] = cy + yFactor * y;
		emitVertex( v );
	}
}


void GLGraphics2D::emitLineVertex(Vec2f const &p1, Vec2f const &p2)
{
	emitVertex( p1.x , p1.y );
	emitVertex( p2.x , p2.y );
}

void GLGraphics2D::emitVertex(float x , float y)
{
	mBuffer.push_back( x );
	mBuffer.push_back( y );
}

void GLGraphics2D::emitVertex( float v[] )
{
	mBuffer.push_back( v[0] );
	mBuffer.push_back( v[1] );

}

void GLGraphics2D::drawPixel(Vec2f const& p , ColorKey3 const& color)
{
	glBegin( GL_POINTS );
	glVertex2i( p.x , p.y );
	glEnd();
}

void GLGraphics2D::drawRect(int left , int top , int right , int bottom)
{
	Vec2f p1( left , top );
	Vec2f p2( right , bottom );
	DRAW_IMPL( emintRectVertex( p1 , p2 ) );
}

void GLGraphics2D::drawRect(Vec2f const& pos , Vec2f const& size)
{
	Vec2f p2 = pos + size;
	DRAW_IMPL( emintRectVertex( pos , p2 ) );
}

void GLGraphics2D::drawCircle(Vec2f const& center , int r)
{
	int numSeg = std::max( 2 * r * r , 4 );
	DRAW_IMPL( emitCircleVertex( center.x , center.y , r , calcCircleSemgmentNum( r ) ) );
}

void GLGraphics2D::drawEllipse(Vec2f const& center , Vec2f const& size)
{
	float yFactor = float( size.y ) / size.x;
	DRAW_IMPL( emitEllipseVertex( center.x , center.y , size.x , yFactor , calcCircleSemgmentNum( ( size.x + size.y ) / 2 ) ) );
}

void GLGraphics2D::drawPolygon(Vec2f pos[] , int num)
{
	DRAW_IMPL( emitPolygonVertex( pos , num ) );
}

void GLGraphics2D::drawPolygon(Vec2i pos[] , int num)
{
	DRAW_IMPL( emitPolygonVertex( pos , num ) );
}

void GLGraphics2D::drawLine(Vec2f const& p1 , Vec2f const& p2)
{
	DRAW_LINE_IMPL( emitLineVertex(p1, p2) );
}

void GLGraphics2D::drawRoundRect(Vec2f const& pos , Vec2f const& rectSize , Vec2f const& circleSize)
{
	DRAW_IMPL( emitRoundRectVertex( pos , rectSize , circleSize / 2 ) );
}

void GLGraphics2D::setTextColor(uint8 r , uint8 g, uint8 b)
{
	mColorFont = ColorKey3( r , g , b );
}

void GLGraphics2D::drawText(Vec2f const& pos , char const* str)
{
	if ( !mFont )
		return;
	int fontSize = mFont->getSize();
	float ox = pos.x;
	float oy = pos.y + fontSize;
	drawTextImpl(ox, oy, str);
}

void GLGraphics2D::drawText(Vec2f const& pos , Vec2f const& size , char const* str , bool beClip /*= false */)
{
	if ( !mFont )
		return;
	int fontSize = mFont->getSize();
	int strLen = strlen( str );
	float ox = pos.x + ( size.x - ( 3 * fontSize / 4 ) * strLen ) / 2 ;
	float oy = pos.y + fontSize + ( size.y - fontSize ) / 2;
	drawTextImpl(ox, oy, str);
}

void GLGraphics2D::drawTextImpl(float  ox, float  oy, char const* str)
{
	assert( mFont );
	glColor4ub( mColorFont.r , mColorFont.g , mColorFont.b , mAlpha );
	glRasterPos2i(0, 0);
	glBitmap(0, 0, 0, 0, ox , -oy , NULL);
	mFont->print( str );
}

void GLGraphics2D::drawTexture(GLTexture& texture , Vec2f const& pos)
{

}

void GLGraphics2D::drawTexture(GLTexture& texture , Vec2f const& pos , ColorKey3 const& color)
{

}

void GLGraphics2D::drawTexture(GLTexture& texture , Vec2f const& pos , Vec2f const& texPos , Vec2f const& texSize)
{

}

void GLGraphics2D::drawTexture(GLTexture& texture , Vec2f const& pos , Vec2f const& texPos , Vec2f const& texSize , ColorKey3 const& color)
{

}

void GLGraphics2D::beginBlend(Vec2f const& pos , Vec2f const& size , float alpha)
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
	mAlpha = std::min( 255u , unsigned( alpha * 255 ) );
}

void GLGraphics2D::endBlend()
{
	glDisable( GL_BLEND );
	mAlpha = 255;
}

#ifdef SYS_PLATFORM_WIN
void GLFont::buildBaseImage( int size , char const* faceName , HDC hDC )
{
	mSize = size;

	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	mIdBaseList = glGenLists(96);								// Storage For 96 Characters

	int height = -(int)(fabs( ( float)10 * size *GetDeviceCaps( hDC ,LOGPIXELSY)/72)/10.0+0.5);
	

	font = CreateFontA(	
		height ,					    // Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_BOLD,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
		faceName );			    // Font Name

	
	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, mIdBaseList);	    // Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}
#endif
void GLFont::printf(const char *fmt, ...)
{
	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_list	ap;	
	char    text[1024];								// Holds Our String

	va_start(ap, fmt);									// Parses The String For Variables
	vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text


	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(mIdBaseList - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}


void GLFont::print( char const* str )
{
	if (str == NULL)									// If There's No Text
		return;											// Do Nothing
	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(mIdBaseList - 32);								// Sets The Base Character to 32
	glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}


GLFont::~GLFont()
{
	release();
}

void GLFont::create( int size , char const* faceName , HDC hDC)
{
	buildBaseImage( size , faceName , hDC );
}

void GLFont::release()
{
	if ( mIdBaseList )
	{
		glDeleteLists(mIdBaseList, 96);
		mIdBaseList = 0;
		mSize = 0;
	}
}
