/*
	CSE 102 - SUMMER HOMEWORK
	131044023 - MUHAMMED SELIM DURSUN
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define WIDTH 460 			 /* width of screen */
#define HEIGHT 710 	 		/* height of screen */
#define FPS 60  		 		/* frame per second */
#define ONE_SIDE_R 59 	 		/* Side of rectangular */
#define ONE_SIDE_T 56 		/* Side of triangle */
#define HOR_OBJECT_NUM 7 		/* Number Of horizontal object*/
#define VER_OBJECT_NUM 9 		/* Number Of vertical object */
#define CIRCLE_RADIUS 8 		/* Ball's Radius */
#define MAX_BALL_SPEED 8.5 		/* Ball's x or y axis's max */
#define MIN_BALL_SPEED 8		/* Ball's x or y axis's min */
#define MAX_POSSIBILITY 112649 /* max pos. in object.txt */
#define ADD_DESTROYER_R 17.5/* adding ball ve destroyer radius*/
#define BOUNCE_GAP 9     /* it should be max ball speed + object thickness */
#define OBJECT_THICKNESS 3 /* objects's(triangles and rectangulars) thinckness*/
#define OBJECT_SCROLL_STEP 66 


#define SCORES_TXT "objects/txts/scores.txt"
#define OBJECTS_TXT "objects/txts/object.txt"


/*this variables changing when in game loops */
int LEVEL = 1; /*The initial number of level*/
int NUMBER_OF_BALL = 1;  /*The initial number of balls*/
int REMAINING_BALL = 0; /*number of will send ball*/
int TOP_SCORE = 0; /*maximum level between played games*/
int SOUND = 1 ; /*1 is sound_on - 0 is sound_off */
bool CLOSE = false; /* if true stops all loop */
int NEW_INDEX = 0; /* The index of first ball arrived to the starting line */
int NEW_BALL = 0; /*The number of add ball collected in a round*/
int FRAMES = 0; /*total frame number the beginning*/
int VISIBLE = 0; /*flashing efect variable*/
double new_ball_x = 230, new_ball_y = 640; /*The initial position of the balls*/



typedef struct mouse {

	double first_mouse_x, first_mouse_y; /*when initialize mouse click down*/
	double second_mouse_x, second_mouse_y; /*when initialize mouse click up*/
} mouse_t;     
/*
	side 1 > up to left
	side 2 > down to left
	side 3 > up to right
	side 4 > down to right
	side 5 > down to up
	side 6 > up to down
	side 7 > right to left
	side 8 > left to right
*/
typedef struct ball{
	int side; /* direction the ball will go */
	double x; /* ball's X location on map */
	double y; /* ball's Y location on map */
	double axis_x; /* the way the ball will be in the x-axis */
	double axis_y; /* the way the ball will be in the y-axis */
	bool life; 	/* if ball in map and active or inactive */
	bool destroy; /* is ball go into the destroyer */
	bool circulation;
} ball_t;

enum ball_life{ DIE, LIVING};

/*
	object>>everything on map (squares,triangles,ball adder,destroyer)
	object ID 0 > square
			ID 1 > triangle, hypotenuse facing left
			ID 2 > triangle, hypotenuse facing right
			ID 3 > ball adder
			ID 4 > horizontal destroyer
			ID 5 > vertical destroyer
*/
typedef struct object
{
	int x; /* object's X location on map */
	int y; /* object's Y location on map */
	int health; /* object's health */
	int ID; /* object's ID () */
	bool visible; /* if ball bounce it, object is shine momentary*/
} object_t;

/* The man throws the ball */
typedef struct man
{
	int x; /* Man bitmap's X location */
	int y; /* Man bitmap's Y location */
} man_t;

/* The coordinates of number of balls remaining to will be send */
typedef struct remaining_ball
{
	int x;
	int y;
} remaining_ball_t;




int menu();

/*
void game_loop();
	updating main variables (levels, ball number vs) and call inner_game_loop func.
	allocating memory for ball structure
*/
void game_loop();

/*	
void inner_game_loop(ball_t *ball, remaining_ball_t remaining_ball, man_t *man);

	The main operations are done here; sending the ball process, balls 
	collision with objects and walls, balls moving, scroll object...
*/
void inner_game_loop(ball_t *ball, remaining_ball_t remaining_ball, man_t *man);


/*
void new_level_scheme();
	
	Pc keep the number of rand and takes mode by MAX_POSSIBILITY (as defined 112649)
	and read object.txt (previously created, txt layout as follows below)
	if read number equal kept number, read follows seven number.
	
	if number 0 or 1, will draw space
	if number 2 or 3, will draw square
	if number 4, will draw triangle
	if number 5, will draw ball adder
	if number 6, will draw horizontal destroyer
	if number 7, will draw vertical destroyer
	
	According to this, init object ID and coordinates.
		
		Squares ID 0
		Triangles ID 1
		Ball Adder ID 2
		Vertical or Horizontal destroyer ID 3
		
	 object.txt layout
	 
	 all sequence have max two triangles
	 all sequence have at least one squares
	 all sequence have at least 3 object, most 7 object
	 all sequence have one ball adder
	 all sequence have max one destroyer and one out of every five 
	 									(destroyer in which mode 5)			 	 					
*/
void new_level_scheme();

/*
void init_mouse_axis( mouse_t *mouse );
	init all mouse struct data
*/
void init_mouse_axis( mouse_t *mouse );

/*
void init_ball( ball_t *ball );
	init all ball struct data
*/
void init_ball( ball_t *ball );

/*
void init_object();
	init all objects's data
*/
void init_object();


/*
void init_colors_fonts_bitmaps_voices();
	init all colors, fonts, bitmaps and voices
*/
void init_colors_fonts_bitmaps_voices();


/* 
void find_top_score();
	this function read scores.txt and find the high score 
*/
void find_top_score();

/*
void scroll_object_id();
	Carries all the object data to a lower row at the end of each round
*/
void scroll_object_id();


/*
void ball_speed_fixing( ball_t *ball );
 	ball's speed should be between BOUNCE_GAP and MIN_BALL_SPEED 
	that function fix all ball's speed
*/
void ball_speed_fixing( ball_t *ball );

/*	
void color_changer( ALLEGRO_COLOR *color, int health );
	change pointer color by the number of objects
*/
void color_changer( ALLEGRO_COLOR *color, int health );

/*
void draw_text(remaining_ball_t remaining_ball);
	draw all required text and numbers
*/
void draw_text(remaining_ball_t remaining_ball);

/* 
double distance_func( int x1, int y1, int x2, int y2 );
	calculate between two coordinates 
	
	return double distance of between two coordinates
*/
double distance_func( int x1, int y1, int x2, int y2 );

/* 
void change_location( ball_t *ball );
	this function change ball's current location by ball's side
*/
void change_location( ball_t *ball );

/*
void adding_ball_control( ball_t *ball, int i, int j );
	check that ball is within adding ball obje coordinates or not.
	
	if yes, a number of balls is increased
*/
void adding_ball_control( ball_t *ball, int i, int j );

/*
void square_ball_control( ball_t *ball, int i, int j );
	check between square and ball distance 
	
	if two objects touch, change ball's side and 1 point reduce square health 
*/
void square_ball_control( ball_t *ball, int i, int j );

/*
void triangle_ball_control( ball_t *ball, int i, int j );
	check between triangle and ball distance 
	
	if two objects touch, change ball's side and 1 point reduce square health 
*/
void triangle_ball_control( ball_t *ball, int i, int j );
/*
void destroyer_control( ball_t *ball, int i, int j );
	check that destroyers did it cut any object or not
*/
void destroyer_control( ball_t *ball, int i, int j );
/*
void is_destroyer_used();
	check that any ball exist on the destroyer and cut any object
*/
void is_destroyer_used();

/*
void bounce_from_wall( ball_t *ball );
	control that ball bounce wall or not
	if ball enough near to wall, bounce as it's supposed to be
*/
void bounce_from_wall( ball_t *ball );
/*
int is_balls_inlife(ball_t *ball);
	check that are all balls DIE or not.
	if all balls die;
				return 0;
	else
				return 1; 
*/
int is_balls_inlife(ball_t *ball);

/*
int is_game_over();
	check game is over or not
	return 1 game not over
	return 0 game over
*/
int is_game_over();

/*
void destroy_process();
	destroy all allegro proccess
*/
void destroy_process();




ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_FONT *whitrabt_font30 = NULL;
ALLEGRO_FONT *batman_font23 = NULL;
ALLEGRO_FONT *arial_font20 = NULL;
ALLEGRO_FONT *arial_font17 = NULL;
ALLEGRO_FONT *whitrabt_font35 = NULL;
ALLEGRO_MOUSE_STATE state;
ALLEGRO_BITMAP *player = NULL;
ALLEGRO_BITMAP *game_backround = NULL;
ALLEGRO_BITMAP *bitmap_ball = NULL;
ALLEGRO_BITMAP *destroyer_hor = NULL;
ALLEGRO_BITMAP *destroyer_ver = NULL;
ALLEGRO_BITMAP *adding_ball = NULL;
ALLEGRO_BITMAP *menu_backround = NULL;
ALLEGRO_BITMAP *menu_man = NULL;
ALLEGRO_BITMAP *play_again = NULL;
ALLEGRO_BITMAP *restart_backround = NULL;
ALLEGRO_BITMAP *exit_door = NULL;
ALLEGRO_BITMAP *sound_on = NULL;
ALLEGRO_BITMAP *sound_off = NULL;


ALLEGRO_SAMPLE *bounce_audio = NULL;
ALLEGRO_SAMPLE *game_over_audio = NULL;
ALLEGRO_SAMPLE *destroy_audio = NULL;
ALLEGRO_SAMPLE *add_ball_audio = NULL;
ALLEGRO_SAMPLE *entry_audio = NULL;
ALLEGRO_SAMPLE *mouse_on_button_audio = NULL;
ALLEGRO_SAMPLE *transitions_audio = NULL;

ALLEGRO_COLOR yellow;
ALLEGRO_COLOR light_yellow;
ALLEGRO_COLOR orange;
ALLEGRO_COLOR light_orange;
ALLEGRO_COLOR light_green;
ALLEGRO_COLOR red1;
ALLEGRO_COLOR red2;
ALLEGRO_COLOR red3;
ALLEGRO_COLOR red4;
ALLEGRO_COLOR red5;
ALLEGRO_COLOR red6;
ALLEGRO_COLOR blue1;
ALLEGRO_COLOR blue2;
ALLEGRO_COLOR blue3;
ALLEGRO_COLOR blue4;
ALLEGRO_COLOR purple;
ALLEGRO_COLOR cyan;
ALLEGRO_COLOR black;
ALLEGRO_COLOR white;
ALLEGRO_COLOR greenish_yellow;
ALLEGRO_COLOR spring_green;



/* 
	object: triangles, squares, ball adder, destroyer
	object_t structure carries this objects's datas
	
	globally defined as it used for each function
*/
object_t object[VER_OBJECT_NUM][HOR_OBJECT_NUM];

int main( int argc, char **argv ){

	srand( time( NULL ) );

	/* init and install allegro event */
	if( !al_init() ) 
	{
		printf( "failed to initialize allegro!\n" );
	}
	/* creating display */
	display = al_create_display( WIDTH, HEIGHT );
	
	if( !display ) {
  		printf( "failed to create display!\n" );
		return -1;
	}
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_image_addon();
	
	al_install_mouse();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	
	init_object();
	init_colors_fonts_bitmaps_voices();	

	/* frequency will play at some time */
	al_reserve_samples( 10 ); 
	
	if( menu() )
		game_loop();
	
	/* destroy process */	
	destroy_process();

	return 0;
} /*end of main.c */



int menu()
{
	bool done = false; /*loop control variable*/ 
	int man_x = 150, man_y = 116; /* moving man's starting coordinates */
	int ball_x = 355, ball_y = 430; /* moving ball's starting coordinates */
	bool m_left = false, m_right = true; /* limits of man's moving */
	bool b_down = false, b_up = true; /* limits of ball's moving */
	bool mouse_button = false; /* if mouse in play key, key's around shines */
	
	event_queue = al_create_event_queue();	
	
	if( !event_queue ) {
		printf( "failed to create event_queue!\n" );
  		return -1;
	}
 
 	find_top_score();
	timer = al_create_timer( 1.0 / FPS );
	
	al_register_event_source(event_queue, al_get_mouse_event_source() );
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	
	al_start_timer( timer );
	
	do
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event( event_queue, &ev );
		al_get_mouse_state( &state );
		
		if( ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
		{
			done = true;
			al_play_sample(game_over_audio, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			al_rest( 0.3 );
		}

		/*if click play button by mouse*/
		if( ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button & 1)
		{	
			if( ev.mouse.x > 254 && ev.mouse.x < 392 && 
				ev.mouse.y > 499 && ev.mouse.y < 562 )
			{
				al_play_sample(entry_audio, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0 );
				return 1;
			}
		}

		
		if( al_is_event_queue_empty( event_queue ) )
		{	
			/*man is moving*/
			if( man_x != 50 && !m_left)
				man_x -= 2;
			else 
			{
				m_left = true;
				m_right = false;
			}
			if( man_x != 344 && !m_right )
				man_x += 2;
			else 
			{
				m_left = false;
				m_right = true;
			}
		
			/*ball is moving*/
			if( ball_y != 490 && !b_down )
				ball_y += 4;
			else 
			{
				b_down = true;
				b_up = false;
			}
			if( ball_y != 430 && !b_up )
				ball_y -= 3;
			else 
			{
				b_down = false;
				b_up = true;
			}

			al_draw_bitmap( menu_backround, 0, 0, 0 );
			/*play button coordinates*/
			/*if mouse on play button coordinates */
			if(state.x > 254 && state.x < 392 && state.y > 499 && state.y < 562)
			{
				al_draw_rectangle( 254, 499, 392, 562, 
									greenish_yellow, 5 );
				if( !mouse_button )
				{
					al_play_sample( mouse_on_button_audio, 1, 0, 1, 
									ALLEGRO_PLAYMODE_ONCE, 0 );
					mouse_button = true;
				}
			}
			else
				mouse_button = false;
		
			al_draw_bitmap( menu_man, man_x, man_y, 0 );
			al_draw_filled_circle( ball_x, ball_y, CIRCLE_RADIUS, white );

			al_flip_display();
			al_clear_to_color(black);
		}
	}while( !done );
}

/*
	updating main variables (levels, ball number vs) and call inner_game_loop func.
	allocating memory for ball structure
*/
void game_loop()
{
	FILE *inp_score;
	double distance;
	bool done = false;
	bool key = false;
	bool mouse_button = false;
	

	ball_t *ball;
	man_t man;
	remaining_ball_t remaining_ball;
	
	/* man's coordinates set specially the best position */
	man.x = new_ball_x - 2;
	man.y = new_ball_y - 55;
	
	timer = al_create_timer( 1.0 / FPS );
	/* register event source */
	al_register_event_source( event_queue, al_get_mouse_event_source());
	al_register_event_source( event_queue, al_get_timer_event_source(timer));
	al_register_event_source( event_queue, al_get_display_event_source(display));
	
	al_start_timer(timer);

	/* allocating memory */
	ball = ( ball_t* )malloc( 300*sizeof( ball_t ) );
	
	init_ball(ball);
	
	find_top_score();
	do{
		/* number of the will send ball left the man */
		REMAINING_BALL = NUMBER_OF_BALL;
		remaining_ball.x = ball[NEW_INDEX].x;;
		remaining_ball.y = ball[NEW_INDEX].y;
		
		/*reallocating*/
		if( LEVEL % 250 == 0 )
		{
			ball = (ball_t*)realloc(ball, 250*sizeof(ball_t));
		}
		
		/*init new level objects ID*/
		new_level_scheme();
		/*all objects scrool 1 step by horizontal */
		scroll_object_id();
		/*if ball in game area and active, this func is working*/
		inner_game_loop(ball, remaining_ball, &man);
		/*next level*/
		LEVEL++;
		/* if how many ball collect, so it should improve ball numbers */
		NUMBER_OF_BALL += NEW_BALL;
		
		NEW_BALL = 0;

	} while( is_game_over() );
	
	al_rest( 0.8 );
	
	/*all scores saving in a file, appeding score after game over*/
	inp_score = fopen(SCORES_TXT, "a");
	
	fprintf( inp_score, "%d\n", LEVEL - 1 );
	
	fclose( inp_score );
	
	if( CLOSE ) return;
	
	/*
	will play screen when game over. 
	User have to decide 'play again' or 'exit' game 
	*/
	do{
		
		ALLEGRO_EVENT ev;
		al_wait_for_event( event_queue, &ev );
		al_get_mouse_state( &state );
		
		/* distance between mouse and play again key */
		distance = sqrt( pow( state.x - 225, 2 ) + pow( state.y - 360, 2 ) );
		
		if( ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
		{
			done = true;	
			if( SOUND )
			{
				al_play_sample( game_over_audio, 1, 0, 1, 
								ALLEGRO_PLAYMODE_ONCE, 0 );
				al_rest( 0.3 );
			}
		}
		
		else if( ev.type == ALLEGRO_EVENT_TIMER )
		{
			key = true;
			al_draw_bitmap( restart_backround, 0, 0, 0 );
			al_draw_bitmap( play_again, 140, 250, 0 );
			al_draw_bitmap( exit_door, 184, 580, 0 );

			/*if mouse on exit button*/
			if( state.x > 188 && state.x < 276 && state.y > 583 && state.y < 710 )
			{
				al_draw_rectangle( 188, 580, 273, 710, 
									greenish_yellow, 5 );
				if( !mouse_button && SOUND )
				{
					al_play_sample( mouse_on_button_audio, 1, 0, 1, 
									ALLEGRO_PLAYMODE_ONCE, 0 );
					mouse_button = true;
				}
			}
			/*if mouse on play button */
			else if( distance <= 60 )
			{
				al_draw_circle( 225, 360, 60, spring_green, 10 );
				if( !mouse_button && SOUND )
				{
					al_play_sample( mouse_on_button_audio, 1, 0, 1, 
									ALLEGRO_PLAYMODE_ONCE, 0 );
					mouse_button = true;
				}	
			}
			else 
				mouse_button = false;
		}
		
		if( ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN )
		{	
			if( ev.mouse.button & 1 )
			{
				/*if click exit button*/
				if( state.x > 188 && state.x < 276 && 
					state.y > 583 && state.y < 710 )
				{
					done = true;
					if( SOUND )
					{
						al_play_sample( game_over_audio, 1, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );
						al_rest( 0.3 );
					}
				}
				/*if click play again button*/
				else if( distance <= 60 )
				{
					done = true;
					init_object();
					LEVEL = 1;
					NUMBER_OF_BALL = 1;
					
					game_loop(ball);
				
					return;
				}
			}					
		}
		if( key && al_is_event_queue_empty( event_queue ) )
		{
			key = false;	
			al_flip_display();
			al_clear_to_color(black);
		}
	}while( !done );
	
	free(ball);
}


/*	
	The main operations are done here; sending the ball process, balls 
	collision with objects and walls, balls moving, scroll object...
*/
void inner_game_loop(ball_t *ball, remaining_ball_t remaining_ball, man_t *man)
{
	int j, i, k, l, m; /*loop control variables*/
	int distance; /*distance between balls and objects*/
	bool key = false; /* provide that loop working yield */
	bool done = false; /*loop termination variable */
	bool flag = false; /*permisson var. for check that ball die or alive*/
	bool permisson = false; /*give permisson after objects scroll*/
	bool GAME = false; /* check that all balls send or not */
	bool TICKET = false; /*check man is where it should be or not*/
	bool transitions = false; /*transition sound control variable*/
	bool flag2 = false;

	ALLEGRO_COLOR color; /*objects's color*/
	mouse_t mouse;
	
	init_mouse_axis( &mouse );
	init_ball( ball );

	/*initialization*/
	i = 0;
	j = 0;
	k = 0;
	l = 0;
	m = 0;
	
	do 
	{	
		ALLEGRO_EVENT ev;
		al_wait_for_event( event_queue, &ev );
		al_get_mouse_state( &state );
		
		if( ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
		{
			done = true;
			CLOSE = true;		
		}
		
		/*objects scrolling*/
		if( permisson == false )
		{
			if( !transitions && SOUND )
			{
				al_play_sample( transitions_audio, 3, 0, 1, 
								ALLEGRO_PLAYMODE_ONCE, 0 );
				transitions = true;
			}	
			for( i = 0; i < VER_OBJECT_NUM; i++ )
			{
				for( j = 0; j < HOR_OBJECT_NUM; j++ )
				{
					if( object[i][j].ID != -1 && 
						object[i][j].y != -1 && 
						object[i][j].x != -1 )
					{
						object[i][j].y += 1;
					}			
				}		
			}
			m++;
		}
		
		if( m == OBJECT_SCROLL_STEP )
		{
			if( !is_game_over() )
				done = true;
			else
				permisson = true;
		}

		if( permisson )
		{ 
			if( ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN )
			{
				if( ev.mouse.button & 1 )
				{
					mouse.first_mouse_x = ev.mouse.x;
					mouse.first_mouse_y = ev.mouse.y;
				}
				if( ev.mouse.button & 1 && 
			 		state.x > 5 && state.x < 50 && 
			 		state.y > 5 && state.y < 50 )
				{
					if( SOUND ) 
						SOUND = 0;
					else
						SOUND = 1;
				}				
			}
			else if( ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP )
			{							
				if( ev.mouse.button & 1 )
				{
					mouse.second_mouse_x = ev.mouse.x;
					mouse.second_mouse_y = ev.mouse.y;
					flag = true;
				}
			}
		}
		if( ev.type == ALLEGRO_EVENT_TIMER )
		{
			key = true;
			if( mouse.first_mouse_x != 0 && mouse.second_mouse_x != 0 )
			{
				if( mouse.second_mouse_y > mouse.first_mouse_y && 
					ball[0].axis_x == 0 && ball[0].axis_y == 0 )
				{	
					for( j = 0; j < NUMBER_OF_BALL; j++ )
					{
						ball[j].axis_x = ( mouse.first_mouse_x - mouse.second_mouse_x );
						ball[j].axis_y = ( mouse.first_mouse_y - mouse.second_mouse_y );
						ball_speed_fixing( &ball[j] );

						if( mouse.first_mouse_x > mouse.second_mouse_x )
						{
							ball[j].side = 3;
						}
						else if( mouse.first_mouse_x < mouse.second_mouse_x )
						{
							ball[j].side = 1;
						}
						else if( mouse.first_mouse_x == mouse.second_mouse_x && 
								 mouse.first_mouse_y < mouse.second_mouse_y )
						{
							ball[j].side = 5;
						}
					}
	
					init_mouse_axis( &mouse );
				}
				else
					init_mouse_axis( &mouse );
			}
		}
	
		if( key && al_is_event_queue_empty( event_queue ) )
		{
			FRAMES++;
			key = false;
			al_draw_bitmap( game_backround, 0, 0, 0 );
			
			if( FRAMES % 6 == 0 && ball[0].side!= -1 && l < NUMBER_OF_BALL )
			{
				ball[l].life = LIVING;
				ball[l].circulation = true;;
				REMAINING_BALL -= 1;
 	 			l++;		
			}
			if( l == NUMBER_OF_BALL )
			{ 
				GAME = true;
			}
			/*Controls the ball against the whole object here is done
				one by one.
			*/
			for( i = 0; i < NUMBER_OF_BALL; i++ )
			{	
				while(!ball[i].life && i < NUMBER_OF_BALL)
					i++; 
					
				for( k = 1; k < VER_OBJECT_NUM; k++ )
				{				
					for( j = 0; j < HOR_OBJECT_NUM; j++ )
					{
						color_changer( &color, object[k][j].health );
						
						if( object[k][j].ID == 0 && object[k][j].health > 0 )
						{
							
							if( object[k][j].health < 10 )
								al_draw_textf( arial_font20, color, 
												object[k][j].x + 23, 
												object[k][j].y + 15, 0, 
												"%d", object[k][j].health );
							else if( object[k][j].health < 100 )
								al_draw_textf( arial_font20, color, 
												object[k][j].x + 17, 
												object[k][j].y + 15, 0, 
												"%d", object[k][j].health );
							else if( object[k][j].health >= 100 )
								al_draw_textf( arial_font20, color, 
												object[k][j].x + 11, 
												object[k][j].y + 15, 0, 
												"%d", object[k][j].health );
					
							if( ball[i].life == LIVING )
							{
								square_ball_control( &ball[i], k, j );
							}
							if( object[k][j].visible )
							{
								al_draw_rectangle( object[k][j].x, 
													object[k][j].y, 
													object[k][j].x + ONE_SIDE_R, 
													object[k][j].y + ONE_SIDE_R, 
													color, OBJECT_THICKNESS );
							}
							if( !object[k][j].visible && FRAMES == VISIBLE + 1 ) 
							{
								object[k][j].visible = true;
							}	
						}
						else if( object[k][j].ID == 1 && object[k][j].health > 0)
						{
						
							if( object[k][j].health < 10 )
								al_draw_textf( arial_font20, color, 
												object[k][j].x + 38, 
												object[k][j].y + 4, 0, 
												"%d", object[k][j].health );
							else if( object[k][j].health < 100 )
								al_draw_textf( arial_font20, color, 
												object[k][j].x + 31, 
												object[k][j].y + 4, 0, 
												"%d", object[k][j].health );
							else if( object[k][j].health >= 100 )
								al_draw_textf( arial_font20, color, 
												object[k][j].x + 21, 
												object[k][j].y + 4, 0, 
												"%d", object[k][j].health );

							if( ball[i].life == LIVING )
							{
								triangle_ball_control( &ball[i], k, j );
							}
							if( object[k][j].visible )
								al_draw_triangle( object[k][j].x, 
													object[k][j].y, 
													object[k][j].x + ONE_SIDE_T, 
													object[k][j].y, 
													object[k][j].x + ONE_SIDE_T, 
													object[k][j].y + ONE_SIDE_T, 
													color, OBJECT_THICKNESS );
							if( !object[k][j].visible  && FRAMES == VISIBLE + 1 ) 
							{
								object[k][j].visible = true;
							}
							
						}
						else if( object[k][j].ID == 2 && object[k][j].health > 0 )
						{
							al_draw_textf( arial_font20, color, 
											object[k][j].x + 3, 
											object[k][j].y + 3, 0, 
											"%d", object[k][j].health );
									
							if( ball[i].life == LIVING )
							{
								triangle_ball_control( &ball[i], k, j );
							}
							if( object[k][j].visible == true )
								al_draw_triangle( object[k][j].x, 
												object[k][j].y, 
												object[k][j].x + ONE_SIDE_T, 
												object[k][j].y, object[k][j].x, 
												object[k][j].y + ONE_SIDE_T, 
												color, OBJECT_THICKNESS );
							if( !object[k][j].visible  && FRAMES == VISIBLE + 1) 
							{
								object[k][j].visible = true;
							}
						}
						
						else if( object[k][j].ID == 3 )
						{
							al_draw_bitmap( adding_ball, object[k][j].x, 
											object[k][j].y, 0 );

							if( ball[i].life == LIVING )
							{
								adding_ball_control( &ball[i], k, j );
							}				
						}
						else if( object[k][j].ID == 4 )
						{
							al_draw_bitmap( destroyer_hor, object[k][j].x, 
											object[k][j].y, 0 );
							
							if( ball[i].life == LIVING )
							{
								destroyer_control( &ball[i], k, j );
							}			
						}
						else if( object[k][j].ID == 5 )
						{
							al_draw_bitmap( destroyer_ver, object[k][j].x, 
											object[k][j].y, 0 );
							
							if( ball[i].life == LIVING )
							{
								destroyer_control( &ball[i], k, j );
							}				
						}
						/* Init the deceased objects*/
						else if(object[k][j].ID==0 || object[k][j].ID==1 || 
								object[k][j].ID==2)
						{
							if(object[k][j].health<=0)
							{
								object[k][j].x = -1;
								object[k][j].y = -1;
								object[k][j].ID = -1;
								object[k][j].health = -1;
							}
						}			
					}			
				}	
			}
			/*ball's moving and ball - wall bounce controls*/
			for( i = 0; i < NUMBER_OF_BALL; i++ )
			{
				if( ball[i].life == LIVING )
				{
					bounce_from_wall( &ball[i] );

					change_location( &ball[i] );	
				}

				al_draw_filled_circle( ball[i].x, ball[i].y, CIRCLE_RADIUS, white );

				/*if ball circulate, ball's life is LIVING. 
				if ball under the finish line, ball's life is DIE*/
				if( ball[i].y >= 640 && flag )
				{
					ball[i].life = DIE;
					ball[i].y = 640;
					
					/*find the index of first dead ball
					and Man walks where it died
					*/
					if(!flag2 && !ball[i].life && ball[i].circulation )
					{
						flag2 = true;
						NEW_INDEX = i;
					}
				}	
				/* Died ball moving right first dead ball*/
				if(!ball[NEW_INDEX].life && ball[i].circulation)
				{
					if(ball[i].y == ball[NEW_INDEX].y && ball[i].x != ball[NEW_INDEX].x )
					{
						if(ball[i].x > ball[NEW_INDEX].x + 15)
						{
							ball[i].x -= 15;
						}
						else if(ball[i].x < ball[NEW_INDEX].x - 15)
						{
							ball[i].x += 15;
						}
						else 
							ball[i].x = ball[NEW_INDEX].x;
					}				
				}
			}

			/*Man's moving process*/
			if( (*man).x != new_ball_x && (*man).x < new_ball_x - 16 )
				(*man).x  += 15;
			else if( (*man).x != new_ball_x && (*man).x > new_ball_x + 16 )
				(*man).x  -= 15;
			else 
			{
				TICKET = true;
				(*man).x = new_ball_x;
			}
			
			al_draw_bitmap( player, (*man).x, (*man).y, 0 );
			
			/*Sound bitmap*/
			if( SOUND )
				al_draw_bitmap( sound_on, 5, 5, 0 );
			else 
				al_draw_bitmap( sound_off, 10, 10, 0 );
			
			if( !is_balls_inlife(ball) && GAME && TICKET )
			{
				done = true;
				GAME = false;
				TICKET = false;
				is_destroyer_used();
			}
			
			draw_text(remaining_ball);
			
			/*drawing mouse target line*/
			if( mouse.second_mouse_x != state.x && 
				mouse.second_mouse_y != state.y && 
				mouse.first_mouse_x != 0 && 
				mouse.first_mouse_y != 0 && !is_balls_inlife(ball) && TICKET)
			{
				if( state.x - mouse.first_mouse_x > 0 )
					al_draw_line( ball[NEW_INDEX].x, ball[NEW_INDEX].y, 
					ball[NEW_INDEX].x - ( state.x - mouse.first_mouse_x ), 
					ball[NEW_INDEX].y - ( state.y - mouse.first_mouse_y ), 
					white, 3 );
				
				else if( state.x - mouse.first_mouse_x < 0 )
					al_draw_line( ball[NEW_INDEX].x, ball[NEW_INDEX].y, 
					ball[NEW_INDEX].x + ( mouse.first_mouse_x - state.x ), 
					ball[NEW_INDEX].y - ( state.y - mouse.first_mouse_y ), 
					white, 3 );
				
				else if( state.x == mouse.first_mouse_x )
					al_draw_line( ball[NEW_INDEX].x, ball[NEW_INDEX].y, ball[NEW_INDEX].x, 
					ball[NEW_INDEX].y + ( mouse.first_mouse_y - state.y ), 
					white, 3 );
			}
		

			/*
			Copies or updates the front and back buffers so that what has been 
			drawn previously on the currently selected display becomes visible on screen
			*/
			al_flip_display();
			al_clear_to_color(black);
			
		}		
	} while( !done );
	
}

/* this function read scores.txt and find the high score */
void find_top_score()
{
	int top;
	FILE *inp_score = fopen( SCORES_TXT, "r" );
	
	while( fscanf( inp_score, "%d", &top ) != EOF )
	{
		if( top > TOP_SCORE )
			TOP_SCORE = top;
	}
	fclose( inp_score );
}

/*
	check game is over or not
	return 1 game not over
	return 0 game over
*/
int is_game_over()
{
	int i;
	if( CLOSE ) 
	{
		if( SOUND ) al_play_sample( game_over_audio, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0 );
		return 0;
	}
	for( i = 0;i<HOR_OBJECT_NUM;i++ )
	{
		if( object[8][i].ID == 3 || object[8][i].ID == 4 || object[8][i].ID == 5 )
		{
			object[8][i].x = -1;
			object[8][i].y = -1;
			object[8][i].ID = -1;	
		}
		if( object[8][i].ID != -1 )
		{
			if( SOUND ) al_play_sample( game_over_audio, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0 );
			return 0;
		}
	}
	
	return 1;
}

/*
	Carries all the object data to a lower row at the end of each round
*/
void scroll_object_id()
{
	int i, j;
	
	for( i = 7; i >= 0; i-- )
	{
		for( j = 6; j >= 0; j-- )
		{	
			object[i + 1][j].ID = object[i][j].ID;
			object[i + 1][j].x = object[i][j].x;
			object[i + 1][j].y = object[i][j].y;
			object[i + 1][j].health = object[i][j].health;
			object[i + 1][j].visible = object[i][j].visible;
		}
	}
}

/*
	check that ball is within adding ball obje coordinates or not.
	
	if yes, a number of balls is increased
*/
void adding_ball_control( ball_t *ball, int i, int j )
{
	int distance;
			
	distance = distance_func( (*ball).x, (*ball).y, 
							  object[i][j].x + ADD_DESTROYER_R, 
							  object[i][j].y + ADD_DESTROYER_R);
	
	if( distance <= 17 +  CIRCLE_RADIUS )
	{
		NEW_BALL += 1; 
		object[i][j].y = -1;
		object[i][j].x = -1;
		object[i][j].ID = -1;	
		object[i][j].visible = -1;
		object[i][j].health = -1;
		
		if( SOUND ) 
			al_play_sample( add_ball_audio, 1, 0, 1, 
							ALLEGRO_PLAYMODE_ONCE, 0 );				
	}
}

/*
	check that are all balls DIE or not.
	if all balls die;
				return 0;
	else
				return 1; 
*/
int is_balls_inlife(ball_t *ball)
{
	int i, count = 0;
	
	if( ball[NEW_INDEX].y >= 640 ) 
		new_ball_x = ball[NEW_INDEX].x;
		
	for( i = 0; i < NUMBER_OF_BALL; i++ )
	{
		if( ball[i].life == DIE && ball[NEW_INDEX].x == ball[i].x ) 
			count++;
	}
	
	if( count == NUMBER_OF_BALL ) 
		return 0;
	else 
		return 1;
}

/*
	control that ball bounce wall or not
	if ball enough near to wall, bounce as it's supposed to be
	
	side 1 > up to left
	side 2 > down to left
	side 3 > up to right
	side 4 > down to right
	side 5 > down to up
	side 6 > up to down
	side 7 > right to left
	side 8 > left to right
*/
void bounce_from_wall( ball_t *ball )
{
	/*up wall control*/
	if( ( (*ball).side == 5 && (*ball).y <= 68 ) ) (*ball).side = 6; 
	else if( ( (*ball).side == 1 && (*ball).y <= 68 ) ) (*ball).side = 2; 
	else if( ( (*ball).side == 3 && (*ball).y <= 68 ) ) (*ball).side = 4; 
	/*left wall control*/
	else if( ( (*ball).side == 2 && (*ball).x <= 8 ) ) (*ball).side = 4; 
	else if( ( (*ball).side == 1 && (*ball).x <= 8 ) ) (*ball).side = 3; 
	else if( ( (*ball).side == 7 && (*ball).x <= 8 ) ) (*ball).side = 8; 
	/*right wall control*/
	else if( ( (*ball).side == 3 && (*ball).x >= 452 ) ) (*ball).side = 1; 
	else if( ( (*ball).side == 4 && (*ball).x >= 452 ) ) (*ball).side = 2; 
	else if( ( (*ball).side == 8 && (*ball).x >= 452 ) ) (*ball).side = 7; 
}

/* 
	calculate between two coordinates 
	
	return distance of between two coordinates
*/
double distance_func( int x1, int y1, int x2, int y2 )
{
	double distance = 0;
	
	distance = sqrt( 
					pow( x1 - x2, 2 ) + 
					pow( y1 - y2, 2 ) );			
	return distance;		
}

/*
 	ball's speed should be between BOUNCE_GAP and MIN_BALL_SPEED 
	that function fix all ball's speed
*/
void ball_speed_fixing( ball_t *ball )
{
	if( (*ball).axis_x < 0 ) (*ball).axis_x *= -1; 
	if( (*ball).axis_y < 0 ) (*ball).axis_y *= -1; 
	
	if( ( (*ball).axis_x > MAX_BALL_SPEED || (*ball).axis_y > MAX_BALL_SPEED ) )
	{
		while( (*ball).axis_x > MAX_BALL_SPEED || (*ball).axis_y > MAX_BALL_SPEED )
		{
			(*ball).axis_x -= (*ball).axis_x / 10.0;
			(*ball).axis_y -= (*ball).axis_y / 10.0;
		}
	}
	else if( ( (*ball).axis_x < MIN_BALL_SPEED && (*ball).axis_y < MIN_BALL_SPEED ) )
		while( (*ball).axis_x < MIN_BALL_SPEED && (*ball).axis_y < MIN_BALL_SPEED )
		{
			(*ball).axis_x += (*ball).axis_x / 10.0;
			(*ball).axis_y += (*ball).axis_y / 10.0;
		}
}

/* 
	this function change ball's current location by ball's side

	side 1 > up to left
	side 2 > down to left
	side 3 > up to right
	side 4 > down to right
	side 5 > down to up
	side 6 > up to down
	side 7 > right to left
	side 8 > left to right
*/
void change_location( ball_t *ball )
{
	if( (*ball).side == 1 )
	{
		(*ball).x -= (*ball).axis_x ;
		(*ball).y -= (*ball).axis_y ;
	}
	else if( (*ball).side == 2 )
	{
		(*ball).x -= (*ball).axis_x ;
		(*ball).y += (*ball).axis_y ;
	}
	else if( (*ball).side == 3 )
	{
		(*ball).x += (*ball).axis_x ;
		(*ball).y -= (*ball).axis_y ;	
	}
	else if( (*ball).side == 4 )
	{
		(*ball).x += (*ball).axis_x ;
		(*ball).y += (*ball).axis_y ;
	}
	else if( (*ball).side == 5 )
	{
		(*ball).y -= (*ball).axis_y;
	}
	else if( (*ball).side == 6 )
	{
		(*ball).y += (*ball).axis_y;
	}
	else if( (*ball).side == 7 )
	{
		(*ball).y -= (*ball).axis_y;
	}
	else if( (*ball).side == 8 )
	{
		(*ball).y += (*ball).axis_y;
	}
}
/*
	draw all required text and numbers
*/
void draw_text(remaining_ball_t remaining_ball)
{
	if(LEVEL < 10)
		al_draw_textf( whitrabt_font35, white, 224, 20, 0, "%d ", LEVEL );
	else if(LEVEL < 100)
		al_draw_textf( whitrabt_font35, white, 213, 20, 0, "%d ", LEVEL );
	else if(LEVEL < 1000)
		al_draw_textf( whitrabt_font35, white, 205, 20, 0, "%d ", LEVEL );
	else if(LEVEL >= 1000)
		al_draw_textf( whitrabt_font35, white, 195, 20, 0, "%d ", LEVEL );
	
	if( REMAINING_BALL != 0 )
	{
		if( REMAINING_BALL < 10 )
			al_draw_textf( arial_font17, white, 
						   remaining_ball.x - 10, 
						   remaining_ball.y - 29, 0, "x%d ", REMAINING_BALL ); 
		else if( REMAINING_BALL<100 )
			al_draw_textf( arial_font17, white, 
						   remaining_ball.x - 15, 
						   remaining_ball.y - 29, 0, "x%d ", REMAINING_BALL ); 
		else if( REMAINING_BALL >= 100 )
			al_draw_textf( arial_font17, white, 
						   remaining_ball.x - 17, 
						   remaining_ball.y - 29, 0, "x%d ", REMAINING_BALL ); 
	}
	al_draw_text( batman_font23, white, 375, 4, 0, "TOP");
	al_draw_textf( whitrabt_font30, white, 375, 32, 0, "%d", TOP_SCORE );
}

/*
	init all objects's data
*/
void init_object()
{
	int i, j;
	for( i = 0; i < VER_OBJECT_NUM; i++ )
	{
		for( j = 0; j < HOR_OBJECT_NUM; j++ )
		{
			object[i][j].x = -1;
			object[i][j].y = -1;
			object[i][j].ID = -1;
			object[i][j].health = 0;
			object[i][j].visible = true;
		}
	}	
}

/*
	init all mouse struct data
*/
void init_mouse_axis( mouse_t *mouse )
{
	( *mouse ).first_mouse_x = 0;
	( *mouse ).first_mouse_y = 0;
	( *mouse ).second_mouse_x = 0;
	( *mouse ).second_mouse_y = 0;
}

/*
	init all ball struct data
*/
void init_ball( ball_t *ball )
{
	int i;

	for( i = 0; i < NUMBER_OF_BALL; i++ )
	{
		ball[i].x = new_ball_x;
		ball[i].y = new_ball_y;
		ball[i].axis_x = 0.0;
		ball[i].axis_y = 0.0;
		ball[i].side = -1;
		ball[i].life = DIE;
		ball[i].destroy = false;
		ball[i].circulation = false;
	}
}
/*
	init all colors, fonts, bitmaps and voices
*/
void init_colors_fonts_bitmaps_voices()
{
	yellow = al_map_rgb( 255, 241, 100 );
	light_yellow = al_map_rgb( 255, 218, 100 );
	orange = al_map_rgb( 255, 153, 51 );
	light_orange = al_map_rgb( 255, 178, 102 );
	light_green = al_map_rgb( 102, 233, 102 );
	red1 = al_map_rgb( 255, 6, 100 );
	red2 = al_map_rgb( 255, 36, 100 );
	red3 = al_map_rgb( 255, 72, 100 );
	red4 = al_map_rgb( 255, 109, 100 );
	red5 = al_map_rgb( 255, 145, 100 );
	purple = al_map_rgb( 255, 0, 255 );
	blue1 = al_map_rgb( 0, 191, 255 );
	blue2 = al_map_rgb( 0, 178, 238 );
	blue3 = al_map_rgb( 0, 154, 205 );
	blue4 = al_map_rgb( 0, 104, 139 );
	cyan = al_map_rgb( 0, 255, 255 );
	black = al_map_rgb( 0, 0, 0 );
	white = al_map_rgb( 255, 255, 255 );
	greenish_yellow =  al_map_rgb( 219, 232, 120 ); 
	spring_green = al_map_rgb( 0, 238, 118 );	


	whitrabt_font30 = al_load_font( "objects/fonts/whitrabt.ttf", 25, 0 );
	whitrabt_font35 = al_load_font( "objects/fonts/whitrabt.ttf", 35, 0 );
	arial_font20 = al_load_font( "objects/fonts/arial.ttf", 20, 0 );
	arial_font17 = al_load_font( "objects/fonts/arial.ttf", 17, 0 );
	batman_font23 = al_load_font("objects/fonts/batman.ttf", 23,0);
	
	player = al_load_bitmap( "objects/bitmaps/man.png" );
	game_backround = al_load_bitmap( "objects/bitmaps/game_backround.png" );
	bitmap_ball = al_load_bitmap( "objects/bitmaps/mars.png" );
	adding_ball = al_load_bitmap( "objects/bitmaps/adding_ball.png" );
	destroyer_hor = al_load_bitmap( "objects/bitmaps/destroyer_hor.png" );
	destroyer_ver = al_load_bitmap( "objects/bitmaps/destroyer_ver.png" );	
	menu_backround = al_load_bitmap( "objects/bitmaps/menu_backround.jpg" );
	menu_man = al_load_bitmap( "objects/bitmaps/menu_man.png" );
	play_again = al_load_bitmap( "objects/bitmaps/play_again.png" );
	restart_backround = al_load_bitmap( "objects/bitmaps/restart_backround.png" );
	exit_door = al_load_bitmap( "objects/bitmaps/exit_door.png" );
	sound_on = al_load_bitmap( "objects/bitmaps/sound_on.png" );
	sound_off = al_load_bitmap( "objects/bitmaps/sound_off.png" );
	
	bounce_audio = al_load_sample( "objects/audios/bounce_audio.ogg" );
	game_over_audio = al_load_sample( "objects/audios/game_over_audio.ogg" );
	destroy_audio = al_load_sample( "objects/audios/destroy_audio.ogg" );
	add_ball_audio = al_load_sample( "objects/audios/add_ball_audio.ogg" );
	entry_audio = al_load_sample( "objects/audios/entry_audio.ogg" );
	mouse_on_button_audio = al_load_sample( "objects/audios/mouse_on_button_audio.ogg" );
	transitions_audio = al_load_sample( "objects/audios/transitions_audio.ogg" );
}

/*	
	change pointer color by the number of objects
*/
void color_changer( ALLEGRO_COLOR *color, int health )
{
	if( health == 1 )
		*color = light_yellow;
	else if( health == 2 )
		*color = yellow;
	else if( health == 3 )
		*color = light_orange;
	else if( health == 4 )
		*color = red5;
	else if( health == 5 || health == 6 )
		*color = red4;
	else if( health == 7 || health == 8 )
		*color = red3;
	else if( health == 9 || health == 10 )
		*color = red2;	
	else if( health>10 && health<15 )
		*color = red1;
	else if( health == 15 )
		*color = blue1;
	else if( health == 16 )
		*color = blue2;
	else if( health == 17 )
		*color = blue3;
	else if( health >= 18 && health <= 25 )
		*color = orange;
	else if( health >= 26 && health <= 40 )
		*color = light_green;
	else if( health>40 )
		*color = cyan;
}

/*
	check that any ball exist on the destroyer and cut any object
*/
void is_destroyer_used()
{
	int i, j;
	
	for( i = 1; i < VER_OBJECT_NUM - 1; i++ )
	{
		for( j = 0; j < HOR_OBJECT_NUM; j++ )
		{
			if( ( object[i][j].ID == 4 || object[i][j].ID == 5 ) && 
				object[i][j].health == -1 )
			{
				object[i][j].x = -1;
				object[i][j].y = -1;
				object[i][j].ID = -1;
				object[i][j].health = 0;
			}
		}
	}

}

/*
	check that destroyers did it cut any object or not
*/
void destroyer_control( ball_t *ball, int i, int j )
{
	int k;
	int distance;			
	

	distance = distance_func( (*ball).x, (*ball).y, 
								object[i][j].x + ADD_DESTROYER_R, 
								object[i][j].y + ADD_DESTROYER_R);
							
							
	if( distance < ( ADD_DESTROYER_R + CIRCLE_RADIUS ) )
	{
		if( object[i][j].ID == 4 )
		{	
			al_draw_line( 0, object[i][j].y + 18, object[i][j].x + 5, 
							object[i][j].y + 18, yellow, 8 );
			al_draw_line( object[i][j].x + 15, object[i][j].y + 18, 460, 
							object[i][j].y + 18, yellow, 8 );
			if( SOUND ) al_play_sample( destroy_audio, 0.5, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );
			for( k = 0; k < HOR_OBJECT_NUM; k++ )
			{
				if( ( object[i][k].ID == 0 || object[i][k].ID == 1 || 
					object[i][k].ID == 2 ) && object[i][k].health > 0 )
				{	
					object[i][k].health -= 1;	
					object[i][j].health = -1;
				}
			}	
		}
		else if( object[i][j].ID == 5 )
		{
			al_draw_line( object[i][j].x + 18, 60, object[i][j].x + 18, 
							object[i][j].y + 5, yellow, 8 );
			al_draw_line( object[i][j].x + 18, object[i][j].y + 15, 
							object[i][j].x + 18, 650, yellow, 8 );
			if( SOUND ) al_play_sample( destroy_audio, 0.5, 0, 1, 
							ALLEGRO_PLAYMODE_ONCE, 0 );
			for( k = 1; k < VER_OBJECT_NUM - 1; k++ )
			{
				if( ( object[k][j].ID == 0 || object[k][j].ID == 1 || 
						object[k][j].ID == 2 ) && object[k][j].health > 0 )
				{	
					object[k][j].health -= 1;
					object[i][j].health = -1;
				}
			}
		}
	}

}

/*
	check between square and ball distance 
	
	if two objects touch, change ball's side and 1 point reduce square health 
	
	side 1 > up to left
	side 2 > down to left
	side 3 > up to right
	side 4 > down to right
	side 5 > down to up
	side 6 > up to down
	side 7 > right to left
	side 8 > left to right
*/
void square_ball_control( ball_t *ball, int i, int j )
{
	double distance, temp;
	
	distance = distance_func( (*ball).x, (*ball).y, 
								object[i][j].x + ONE_SIDE_R/2, 
								object[i][j].y + ONE_SIDE_R/2 );		
	/* if ball not near square, not necessary these controls */		
	if( distance <= 55 ){
		/*control down of square*/
		if ( (*ball).x >= object[i][j].x - 2 && 
			(*ball ).x <= object[i][j].x + ONE_SIDE_R + 2  && 
			(*ball ).y >= object[i][j].y + ONE_SIDE_R  && 
			(*ball ).y <= object[i][j].y + ONE_SIDE_R + 2 + MAX_BALL_SPEED 	 )
		{	
			if( (*ball).side == 3 )
			{
				(*ball).side = 4;
			}
			else if( (*ball).side == 1 )
			{
				(*ball).side = 2;
			}
			else if( (*ball).side == 5 )
			{
				(*ball).side = 6;	
			}	
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	
			if( SOUND ) 
				al_play_sample( bounce_audio, 1, 0, 1, 
								ALLEGRO_PLAYMODE_ONCE, 0 );
		}
		/*control right of square*/
		else if ( (*ball).x >= object[i][j].x + ONE_SIDE_R && 
				(*ball).x <= object[i][j].x + ONE_SIDE_R + 2 + MAX_BALL_SPEED &&
				(*ball).y >= object[i][j].y - 2 && 
				(*ball).y <= object[i][j].y + ONE_SIDE_R + 2  )
		{	
			if( (*ball).side == 1 )
			{	
				(*ball).side = 3;
			}
			else if( (*ball).side == 2 )
			{
				(*ball).side = 4;	
			}
			else if( (*ball).side == 7 )
			{
				(*ball).side = 8;		
			}
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	
			if( SOUND ) 
				al_play_sample( bounce_audio, 1, 0, 1, 
								ALLEGRO_PLAYMODE_ONCE, 0 );
		}
		/*control left of square*/
		else if ( (*ball).x <= object[i][j].x && 
				  (*ball).x >= object[i][j].x - MAX_BALL_SPEED - 2 &&
				  (*ball).y >= object[i][j].y - 2  && 
				  (*ball).y <= object[i][j].y + ONE_SIDE_R + 2 )
		{	
			if( (*ball).side == 4 )
			{
				(*ball).side = 2;			
			}
			else if( (*ball).side == 3 )
			{
				(*ball).side = 1;			
			}
			else if( (*ball).side == 8 )
			{
				(*ball).side = 7;	
			}
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	
			
			if( SOUND ) 
				al_play_sample( bounce_audio, 1, 0, 1, 
								ALLEGRO_PLAYMODE_ONCE, 0 );	
		}
		/*control up of square*/
		else if ( (*ball).x >= object[i][j].x - 2  &&
				  (*ball).x <= object[i][j].x + ONE_SIDE_R + 2 && 
			 	  (*ball).y <= object[i][j].y  && 
			 	  (*ball).y >= object[i][j].y - MAX_BALL_SPEED - 2 )
		{	
			if( (*ball).side == 4 )
			{
				(*ball).side = 3;			
			}
			else if( (*ball).side == 2 )
			{
				(*ball).side = 1;		
			}
			else if( (*ball).side == 6 )
			{
				(*ball).side = 5;		
			}
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;
			
			if( SOUND ) 
				al_play_sample( bounce_audio, 1, 0, 1, 
								ALLEGRO_PLAYMODE_ONCE, 0 );
		}
		/*control left-up of square*/
		else if( (*ball).side == 4 && distance_func( object[i][j].x , object[i][j].y , 
							  (*ball).x, (*ball).y ) < MAX_BALL_SPEED )
		{	
			if( (*ball).side == 4 )
			{
				(*ball).side = 1;
				temp = (*ball).axis_x ;
				(*ball).axis_x = (*ball).axis_y;
				(*ball).axis_y = temp;
				
				object[i][j].health -= 1;
				object[i][j].visible = false;
				VISIBLE = FRAMES;
			}

			if( SOUND ) 
				al_play_sample( bounce_audio, 1, 0, 1, 
								ALLEGRO_PLAYMODE_ONCE, 0 );
		}
		/*control right-up of square*/
		else if( (*ball).side == 2 && distance_func( object[i][j].x  + ONE_SIDE_R, 
				object[i][j].y,  (*ball).x, (*ball).y ) < MAX_BALL_SPEED )
		
		{	
			if( (*ball).side == 2 )
			{
				(*ball).side = 3;
				temp = (*ball).axis_x ;
				(*ball).axis_x = (*ball).axis_y;
				(*ball).axis_y = temp;
				
				object[i][j].health -= 1;
				object[i][j].visible = false;
				VISIBLE = FRAMES;
			}
	
				
			if( SOUND ) 
				al_play_sample( bounce_audio, 1, 0, 1, 
								ALLEGRO_PLAYMODE_ONCE, 0 );
		}
		/*control left-down of square*/
		else if( (*ball).side == 3 && distance_func( object[i][j].x , 
				object[i][j].y + ONE_SIDE_R, (*ball).x, (*ball).y ) < MAX_BALL_SPEED )
		{	
			if( (*ball).side == 3 )
			{
				(*ball).side = 2;
				temp = (*ball).axis_x ;
				(*ball).axis_x = (*ball).axis_y;
				(*ball).axis_y = temp;
				object[i][j].health -= 1;
				object[i][j].visible = false;
				VISIBLE = FRAMES;	
			}
	
			if( SOUND ) 
				al_play_sample( bounce_audio, 1, 0, 1, 
								ALLEGRO_PLAYMODE_ONCE, 0 );		
		}
		/*control right-down of square*/
		else if( (*ball).side == 1 && distance_func( object[i][j].x + ONE_SIDE_R , 
								object[i][j].y + ONE_SIDE_R , (*ball).x, 
								(*ball).y ) < MAX_BALL_SPEED ) 
		{	
			if( (*ball).side == 1 )
			{
				(*ball).side = 4;
				temp = (*ball).axis_x ;
				(*ball).axis_x = (*ball).axis_y;
				(*ball).axis_y = temp;
				object[i][j].health -= 1;
				object[i][j].visible = false;
				VISIBLE = FRAMES;	
			}
		
				
			if( SOUND ) 
				al_play_sample( bounce_audio, 1, 0, 1, 
								ALLEGRO_PLAYMODE_ONCE, 0 );
		}
	}		
}
/*
	check between triangle and ball distance 
	
	if two objects touch, change ball's side and 1 point reduce square health 
	
	side 1 > up to left
	side 2 > down to left
	side 3 > up to right
	side 4 > down to right
	side 5 > down to up
	side 6 > up to down
	side 7 > right to left
	side 8 > left to right
*/
void triangle_ball_control( ball_t *ball, int i, int j )
{		
	int distance;
	int g_x, g_y;
	double temp;
	g_x = object[i][j].x + ONE_SIDE_T / 3;
	g_y = object[i][j].y + ONE_SIDE_T / 3;

	distance = distance_func( (*ball).x, (*ball).y, g_x, g_y );
	
	if( distance <= 55 && object[i][j].ID == 1 ){
		/*control left-up*/
		if( (*ball).side == 4 && distance_func( object[i][j].x - 2, 
								object[i][j].y , 
								(*ball).x, (*ball).y ) < MAX_BALL_SPEED )
		{	
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	
	
			(*ball).side = 1;
			temp = (*ball).axis_x ;
			(*ball).axis_x = (*ball).axis_y;
			(*ball).axis_y = temp;
			if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );
		}
		/*control right down*/
		else if( (*ball).side == 1 && distance_func( object[i][j].x + ONE_SIDE_T, 
								object[i][j].y + ONE_SIDE_T + 2, 
								(*ball).x, (*ball).y ) < MAX_BALL_SPEED )
		{	
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	
			if( (*ball).side == 1 )
			{
				(*ball).side = 4;
				temp = (*ball).axis_x ;
				(*ball).axis_x = (*ball).axis_y;
				(*ball).axis_y = temp;
			}
			if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );
		}
		/*control right-up*/
		else if( (*ball).side == 2 && distance_func( object[i][j].x + ONE_SIDE_T , 
				object[i][j].y , (*ball).x, (*ball).y ) < MAX_BALL_SPEED )
		{		
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	
			if( (*ball).side == 2 )
			{
				(*ball).side = 3;
				temp = (*ball).axis_x ;
				(*ball).axis_x = (*ball).axis_y;
				(*ball).axis_y = temp;
			}
			if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );	
		}
		/* control up of triangle. */
		else if( (*ball).x >= object[i][j].x - 4  && 
			(*ball).x <= object[i][j].x + ONE_SIDE_T + 4 && 
			(*ball).y <= object[i][j].y  && 
			(*ball).y >= object[i][j].y - 2 - BOUNCE_GAP  )
		{			
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	

			if( (*ball).side == 4 )
			{
				(*ball).side = 3;
			}
			else if( (*ball).side == 2 )
			{
				(*ball).side = 1;
			}
			else if( (*ball).side == 6 )
			{
				(*ball).side = 5;
			}
			if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );	
		}
		/*control right of triangle*/
		else if( (*ball).x >= object[i][j].x  +  ONE_SIDE_T  && 
				 (*ball).x <= object[i][j].x  +  ONE_SIDE_T + 2 +  BOUNCE_GAP && 
				 (*ball).y >= object[i][j].y - 4 && 
				 (*ball).y <= object[i][j].y + 4 + ONE_SIDE_T )
		{	
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	
		
			if( (*ball).side == 2 )
			{
				(*ball).side = 4;
			}
			else if( (*ball).side == 1 )
			{
				(*ball).side = 3;
			}
			else if( (*ball).side == 7 )
			{
				(*ball).side = 8;
			}
			if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );
		}
		/*control hypotenuse of triangle */
		else if(
			distance_func( object[i][j].x - 4, object[i][j].y - 2, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x - 2, object[i][j].y , (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x , object[i][j].y + 2, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 2, object[i][j].y + 4, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 4, object[i][j].y + 6, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 6, object[i][j].y + 8, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 8, object[i][j].y + 10, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 10, object[i][j].y + 12, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 12, object[i][j].y + 14, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 14, object[i][j].y + 16, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 16, object[i][j].y + 18, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 18, object[i][j].y + 20, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 20, object[i][j].y + 22, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 22, object[i][j].y + 24, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 24, object[i][j].y + 26, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 26, object[i][j].y + 28, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 28, object[i][j].y + 30, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 30, object[i][j].y + 32, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 32, object[i][j].y + 34, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 34, object[i][j].y + 36, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 36, object[i][j].y + 38, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 38, object[i][j].y + 40, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 40, object[i][j].y + 42, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 42, object[i][j].y + 44, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 44, object[i][j].y + 46, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 46, object[i][j].y + 48, (*ball).x, (*ball).y ) < MAX_BALL_SPEED || 
			distance_func( object[i][j].x + 48, object[i][j].y + 50, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 50, object[i][j].y + 52, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 52, object[i][j].y + 54, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 54, object[i][j].y + 56, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 56, object[i][j].y + 58, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + 58, object[i][j].y + 60, (*ball).x, (*ball).y ) < MAX_BALL_SPEED )
		{
				object[i][j].health -= 1;
				object[i][j].visible = false;
				VISIBLE = FRAMES;
				
				temp = (*ball).axis_x ;
				(*ball).axis_x = (*ball).axis_y;
				(*ball).axis_y = temp;	

				if( (*ball).side == 3 )
				{
					(*ball).side = 2;
				}				
				if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, 
											ALLEGRO_PLAYMODE_ONCE, 0 );
		}
	}
	
	if( distance <= 55 && object[i][j].ID == 2 )
	{
		/*control right-up*/
		if( (*ball).side == 2 && distance_func( object[i][j].x + ONE_SIDE_T + 2, object[i][j].y , 
							(*ball).x, (*ball).y ) < MAX_BALL_SPEED )
		{		
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	
			if( (*ball).side == 2 )
			{
				(*ball).side = 3;
				temp = (*ball).axis_x ;
				(*ball).axis_x = (*ball).axis_y;
				(*ball).axis_y = temp;
			}
			if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );
		}
		/*control left-up*/
		else if( (*ball).side == 4 && distance_func( (*ball).side == 4 && object[i][j].x , object[i][j].y , 
								(*ball).x, (*ball).y ) < MAX_BALL_SPEED )
		{	
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	
			if( (*ball).side == 4 )
			{
				(*ball).side = 1;
				temp = (*ball).axis_x ;
				(*ball).axis_x = (*ball).axis_y;
				(*ball).axis_y = temp;
			}
			if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );	
		}
		/*control left-down*/
		else if( (*ball).side == 3 && distance_func( (*ball).side == 3 && object[i][j].x , 
				object[i][j].y + ONE_SIDE_T + 2, (*ball).x, (*ball).y ) < MAX_BALL_SPEED )
		{	
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	
			if( (*ball).side == 3 )
			{
				(*ball).side = 2;
				temp = (*ball).axis_x ;
				(*ball).axis_x = (*ball).axis_y;
				(*ball).axis_y = temp;
			}
			if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );	
		}
		/*control up of triangle*/
		else if( (*ball).x >= object[i][j].x - 4  && 
			(*ball).x <= object[i][j].x  +  ONE_SIDE_T + 6 && 
			(*ball).y <= object[i][j].y - 2  && 
			(*ball).y >= object[i][j].y - 2 - BOUNCE_GAP )
		{				
			object[i][j].health  -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	

			if( (*ball).side == 4 )
			{
				(*ball).side = 3;
			}
			else if( (*ball).side == 2 )
			{
				(*ball).side = 1;
			}
			else if( (*ball).side == 6 )
			{
				(*ball).side = 5;
			}
			if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );	
		}
		/*control left of triangle*/
		else if( (*ball).x <= object[i][j].x - 2 && 
				 (*ball).x >= object[i][j].x - 2 - BOUNCE_GAP && 
				 (*ball).y >= object[i][j].y - 4 && 
				 (*ball).y <= object[i][j].y + 4 + ONE_SIDE_T )
		{	
			object[i][j].health -= 1;
			object[i][j].visible = false;
			VISIBLE = FRAMES;	
			if( (*ball).side == 4 )
			{
				(*ball).side = 2;
			}
			else if( (*ball).side == 3 )
			{
				(*ball).side = 1;
			}
			else if( (*ball).side == 8 )
			{
				(*ball).side = 7;
			}	
			if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, 
										ALLEGRO_PLAYMODE_ONCE, 0 );	
		}
		else if( 
			distance_func( object[i][j].x + ONE_SIDE_T + 4,  object[i][j].y - 2 , (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T + 2,  object[i][j].y , (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T ,     object[i][j].y + 2, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 2,  object[i][j].y + 4, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 4,  object[i][j].y + 6, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 6,  object[i][j].y + 8, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 8,  object[i][j].y + 10, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 10, object[i][j].y + 12, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 12, object[i][j].y + 14, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 14, object[i][j].y + 16, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 16, object[i][j].y + 18, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 18, object[i][j].y + 20, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 20, object[i][j].y + 22, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 22, object[i][j].y + 24, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 24, object[i][j].y + 26, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 26, object[i][j].y + 28, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 28, object[i][j].y + 30, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 30, object[i][j].y + 32, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 32, object[i][j].y + 34, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 34, object[i][j].y + 36, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 36, object[i][j].y + 38, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 38, object[i][j].y + 40, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 40, object[i][j].y + 42, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 42, object[i][j].y + 44, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 44, object[i][j].y + 46, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 46, object[i][j].y + 48, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 48, object[i][j].y + 50, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 50, object[i][j].y + 52, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 52, object[i][j].y + 54, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 54, object[i][j].y + 56, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 56, object[i][j].y + 58, (*ball).x, (*ball).y ) < MAX_BALL_SPEED ||
			distance_func( object[i][j].x + ONE_SIDE_T - 58, object[i][j].y + 60, (*ball).x, (*ball).y ) < MAX_BALL_SPEED )
		{	
				object[i][j].health -= 1;
				object[i][j].visible = false;
				VISIBLE = FRAMES;	
				temp = (*ball).axis_x ;
				(*ball).axis_x = (*ball).axis_y;
				(*ball).axis_y = temp;	

				if( (*ball).side == 1 )
				{
					(*ball).side = 4;
				}
				if( SOUND ) al_play_sample( bounce_audio, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0 );	
		}

	}
}
/*
	Pc keep the number of rand and takes mode by MAX_POSSIBILITY (as defined 112649)
	and read object.txt (previously created, txt layout as follows below)
	if read number equal kept number, read follows seven number.
	
	if number 0 or 1, will draw space
	if number 2 or 3, will draw square
	if number 4, will draw triangle
	if number 5, will draw ball adder
	if number 6, will draw horizontal destroyer
	if number 7, will draw vertical destroyer
	
	According to this, init object ID and coordinates.
		
		Squares ID 0
		Triangles ID 1
		Ball Adder ID 2
		Vertical or Horizontal destroyer ID 3
		
	 object.txt layout
	 
	 all sequence have max two triangles
	 all sequence have at least one squares
	 all sequence have at least 3 object, most 7 object
	 all sequence have one ball adder
	 all sequence have max one destroyer and one out of every five 
	 									(destroyer in which mode 5)			 	 					
*/
void new_level_scheme()
{
	FILE *inp; /*file var.*/
	int i; /* loop control var. */
	int read; /* read the number,  estimated is equal */
	int buffer_array[HOR_OBJECT_NUM];
	int number; /*estimated number*/
	
	for( i = 0; i < HOR_OBJECT_NUM; i++ )
	{
		object[0][i].x = -1;
		object[0][i].y = -1;
		object[0][i].ID = -1;
		object[0][i].health = 0;
	}
	inp = fopen( OBJECTS_TXT, "r" );
	
	number = rand()%MAX_POSSIBILITY;
	
	do{
		fscanf( inp, "%d", &read );
	}while( read!= number );

	fscanf( inp, "%d", &buffer_array[0] );
	fscanf( inp, "%d", &buffer_array[1] );
	fscanf( inp, "%d", &buffer_array[2] );
	fscanf( inp, "%d", &buffer_array[3] );
	fscanf( inp, "%d", &buffer_array[4] );
	fscanf( inp, "%d", &buffer_array[5] );
	fscanf( inp, "%d", &buffer_array[6] );
		
	fclose( inp );
	
	if( buffer_array[0] == 2 || buffer_array[0] == 3 )
	{
		object[0][0].x = 5;
		object[0][0].y = 57;
		object[0][0].ID = 0;
		object[0][0].health = LEVEL;
	}
	else if( buffer_array[0] == 4 )
	{
		
		object[0][0].y = 57;
		object[0][0].health = LEVEL;
		
		if( rand()%2 == 0 )
		{
			object[0][0].ID = 1;
			object[0][0].x = 8;
		}
		else
		{
			object[0][0].ID = 2;
			object[0][0].x = 5;
		}
	}
	else if( buffer_array[0] == 5 )
	{
		object[0][0].x = 15;
		object[0][0].y = 72;
		object[0][0].ID = 3;
		object[0][0].health = LEVEL;
	}
	else if( buffer_array[0] == 6 || buffer_array[0] == 7 )
	{
		object[0][0].x = 15;
		object[0][0].y = 72;
		object[0][0].health = LEVEL;
		if( buffer_array[0] == 6 ) object[0][0].ID = 4;
		else if( buffer_array[0] == 7 ) object[0][0].ID = 5;
	}

	if( buffer_array[1] == 2 || buffer_array[1] == 3 )
	{
		object[0][1].x = 70;
		object[0][1].y = 57;
		object[0][1].ID = 0;
		object[0][1].health = LEVEL;
	}
	else if( buffer_array[1] == 4 )
	{
		object[0][1].y = 57;
		object[0][1].health = LEVEL;
		
		if( rand()%2 == 0 )
		{
			object[0][1].ID = 1;
			object[0][1].x = 73;
		}
		else
		{
			object[0][1].ID = 2;
			object[0][1].x = 70;
		}
	}
	else if( buffer_array[1] == 5 )
	{
		object[0][1].x = 80;
		object[0][1].y = 72;
		object[0][1].ID = 3;
		object[0][1].health = LEVEL;
	}
	else if( buffer_array[1] == 6 || buffer_array[1] == 7 )
	{
		object[0][1].x = 80;
		object[0][1].y = 72;
		object[0][1].health = LEVEL;
		if( buffer_array[1] == 6 ) object[0][1].ID = 4;
		else if( buffer_array[1] == 7 ) object[0][1].ID = 5;
	}
	
	if( buffer_array[2] == 2 || buffer_array[2] == 3 )
	{
		object[0][2].x = 135;
		object[0][2].y = 57;
		object[0][2].ID = 0;
		object[0][2].health = LEVEL;
	}
	else if( buffer_array[2] == 4 )
	{
		object[0][2].y = 57;
		object[0][2].health = LEVEL;
		
		if( rand()%2 == 0 )
		{
			object[0][2].ID = 1;
			object[0][2].x = 138;
		}
		else
		{
			object[0][2].ID = 2;
			object[0][2].x = 135;
		}
	}
	else if( buffer_array[2] == 5 )
	{
		object[0][2].x = 145;
		object[0][2].y = 72;
		object[0][2].ID = 3;
		object[0][2].health = LEVEL;
	}
	else if( buffer_array[2] == 6 || buffer_array[2] == 7 )
	{
		object[0][2].x = 145;
		object[0][2].y = 72;
		object[0][2].health = LEVEL;
		if( buffer_array[2] == 6 ) object[0][2].ID = 4;
		else if( buffer_array[2] == 7 ) object[0][2].ID = 5;
	}
	
	if( buffer_array[3] == 2 || buffer_array[3] == 3 )
	{
		object[0][3].x = 200;
		object[0][3].y = 57;
		object[0][3].ID = 0;
		object[0][3].health = LEVEL;
	}
	else if( buffer_array[3] == 4 )
	{
		object[0][3].y = 57;
		object[0][3].health = LEVEL;
		
		if( rand()%2 == 0 )
		{
			object[0][3].ID = 1;
			object[0][3].x = 203;
		}
		else
		{
			object[0][3].ID = 2;
			object[0][3].x = 200;
		}
	}
	else if( buffer_array[3] == 5 )
	{
		object[0][3].x = 210;
		object[0][3].y = 72;
		object[0][3].ID = 3;
		object[0][3].health = LEVEL;
	}
	else if( buffer_array[3] == 6 || buffer_array[3] == 7 )
	{
		object[0][3].x = 210;
		object[0][3].y = 72;
		object[0][3].health = LEVEL;
		if( buffer_array[3] == 6 ) object[0][3].ID = 4;
		else if( buffer_array[3] == 7 ) object[0][3].ID = 5;
	}
	
	if( buffer_array[4] == 2 || buffer_array[4] == 3 )
	{
		object[0][4].x = 265;
		object[0][4].y = 57;
		object[0][4].ID = 0;
		object[0][4].health = LEVEL;
	}
	else if( buffer_array[4] == 4 )
	{
		object[0][4].y = 57;
		object[0][4].health = LEVEL;
		
		if( rand()%2 == 0 )
		{
			object[0][4].ID = 1;
			object[0][4].x = 268;
		}
		else
		{
			object[0][4].ID = 2;
			object[0][4].x = 265;
		}
	}
	else if( buffer_array[4] == 5 )
	{
		object[0][4].x = 275;
		object[0][4].y = 72;
		object[0][4].ID = 3;
		object[0][4].health = LEVEL;
	}
	else if( buffer_array[4] == 6 || buffer_array[4] == 7 )
	{
		object[0][4].x = 275;
		object[0][4].y = 72;
		object[0][4].health = LEVEL;
		if( buffer_array[4] == 6 ) object[0][4].ID = 4;
		else if( buffer_array[4] == 7 ) object[0][4].ID = 5;
	}
	
	if( buffer_array[5] == 2 || buffer_array[5] == 3 )
	{
		object[0][5].x = 330;
		object[0][5].y = 57;
		object[0][5].ID = 0;
		object[0][5].health = LEVEL;
	}
	else if( buffer_array[5] == 4 )
	{
		object[0][5].y = 57;
		object[0][5].health = LEVEL;
		
		if( rand()%2 == 0 )
		{
			object[0][5].ID = 1;
			object[0][5].x = 333;
		}
		else
		{
			object[0][5].ID = 2;
			object[0][5].x = 330;
		}
	}
	else if( buffer_array[5] == 5 )
	{
		object[0][5].x = 340;
		object[0][5].y = 72;
		object[0][5].ID = 3;
		object[0][5].health = LEVEL;
	}
	else if( buffer_array[5] == 6 || buffer_array[5] == 7 )
	{
		object[0][5].x = 340;
		object[0][5].y = 72;
		object[0][5].health = LEVEL;
		if( buffer_array[5] == 6 ) object[0][5].ID = 4;
		else if( buffer_array[5] == 7 ) object[0][5].ID = 5;
	}
	
	if( buffer_array[6] == 2 || buffer_array[6] == 3 )
	{
		object[0][6].x = 395;
		object[0][6].y = 57;
		object[0][6].ID = 0;
		object[0][6].health = LEVEL;
	}
	else if( buffer_array[5] == 4 )
	{
		object[0][6].y = 57;
		object[0][6].health = LEVEL;
		
		if( rand()%2 == 0 )
		{
			object[0][6].ID = 1;
			object[0][6].x = 398;
		}
		else
		{
			object[0][6].ID = 2;
			object[0][6].x = 395;
		}
	}
	else if( buffer_array[6] == 5 )
	{
		object[0][6].x = 405;
		object[0][6].y = 72;
		object[0][6].ID = 3;
		object[0][6].health = LEVEL;
	}
	else if( buffer_array[6] == 6 || buffer_array[6] == 7 )
	{
		object[0][6].x = 405;
		object[0][6].y = 72;
		object[0][6].health = LEVEL;
		if( buffer_array[6] == 6 ) object[0][6].ID = 4;
		else if( buffer_array[6] == 7 ) object[0][6].ID = 5;
	}
}
/*
	destroy all allegro proccess
*/
void destroy_process()
{
	
	al_destroy_display( display );
	
	al_destroy_font( whitrabt_font35 );
	al_destroy_font( whitrabt_font30 );
	al_destroy_font( batman_font23 );
	al_destroy_font( arial_font20 );
	al_destroy_font( arial_font17 );
	
	al_destroy_event_queue( event_queue );
	
	al_destroy_timer( timer );
	
	al_destroy_bitmap( player );
	al_destroy_bitmap( game_backround );
	al_destroy_bitmap( bitmap_ball );
	al_destroy_bitmap( destroyer_hor );
	al_destroy_bitmap( destroyer_ver );
	al_destroy_bitmap( adding_ball );
	al_destroy_bitmap( menu_backround );
	al_destroy_bitmap( menu_man );
	al_destroy_bitmap( play_again );
	al_destroy_bitmap( restart_backround );
	al_destroy_bitmap( exit_door );
	al_destroy_bitmap( sound_on );
	al_destroy_bitmap( sound_off );
	
	al_destroy_sample( bounce_audio );
	al_destroy_sample( game_over_audio );
	al_destroy_sample( destroy_audio );
	al_destroy_sample( add_ball_audio );
	al_destroy_sample( entry_audio );
	al_destroy_sample( mouse_on_button_audio );
	al_destroy_sample( transitions_audio );
}
