#include <lpc21xx.h>

#include "lcd_fun.c" 

#define SOS(x) ( 4000 < x && x < 5000 )

#define HIGH(x) ( 1500 < x && x < 1800 ) 

#define REPEAT(x) ( 95000 < x && x < 100000 ) 

#define CURSOR_RIGHT lcd_cmd( 0x14 )

#define CURSOR_LEFT lcd_cmd( 0x10 )
 
unsigned int start_flag = 0 ;

unsigned int bit_pattern = 0 ;

unsigned int key = 0 ;

unsigned int repeat_key = 0 ;

unsigned int EOS = 0 ;

unsigned int alphanumeric_flag = 1 ;

unsigned int case_flag = 1 ;

unsigned int repeat_count = 0 ;
 
void key_status( void ) ;

unsigned char key_map( void ) ;

void timer_config( void )
{
	T0PR = 14 ;		//load 14 to PR

	T0MR0 = 1000000 ;		//for 1 sec delay ;

	T0MCR = ( 1 << 1 ) ;		//MR0 in repeat mode

	T0TCR = ( 1 << 1 ) ;		//reset the counter to 0

	T0TCR = ( 1 << 0 ) ;	//enable TIMER0
}

int main()
{
	lcd_config() ;

//	uart_config() ;

	timer_config() ;

	while( 1 )
	{
		key_status() ;

		switch( key )
		{
			case 0xE31CFF00 : if( alphanumeric_flag == 0 )
					 			{
					 				alphanumeric_flag = 1 ;
					 			}

							    else
							    {
									alphanumeric_flag = 0 ;
							    }
		
							    break ;

			case 0xF708FF00 : CURSOR_LEFT ;
					
							  break ;

			case 0xA55AFF00 : CURSOR_RIGHT ;
					 
					 		  break ;

			case 0xAD52FF00 : case_flag = 0 ;

					 		  break ;

			case 0xE718FF00 : case_flag = 1 ;

					 		  break ;
					 
			default : lcd_data( key_map())	;
				
					  delay( 500 ) ;
		}
	}
}	

void key_status( void ) 
{
	unsigned int i = 0 ;

	unsigned int timer_value = 0 ;

	while( 1 )
	{
		if((IOPIN1 & ( 1 << 17 )))
		{		
			T0TCR |= ( 1 << 1 ) ;
	
			T0TCR &= ~( 1 << 1 ) ;
	
			while( IOPIN1 & ( 1 << 17 )) ;

			timer_value = T0TC	;
	
			if( SOS( timer_value ))
			{			
				start_flag = 1 ;
	
				bit_pattern = 0 ;

				repeat_key = 0 ;

				EOS = 0 ;

				repeat_count = 0 ;
			}
/*	
			else if( start_flag && i < 32 )
			{
				if( HIGH( timer_value ))
				{
					bit_pattern |= ( 1 << i ) ;
	
					i++ ;
				}
				
				else
				{
					i++ ;
				}
			}
	
			else if( i == 32 )
			{
				key = bit_pattern ;
	
				start_flag = 0 ;
	
				i = 0 ;
	
				EOS = 1 ;
	
				break ;
			}
*/

			else if( start_flag && i < 31 )
			{
				if( HIGH( timer_value ))
				{
					bit_pattern |= ( 1 << i ) ;
	
					i++ ;
				}
				
				else
				{
					i++ ;
				}
			}
	
			else if( i == 31 )
			{
				if( HIGH( timer_value ))
				{
					bit_pattern |= ( 1 << i ) ;
	
//					i++ ;
				}
				

				key = bit_pattern ;
	
				start_flag = 0 ;
	
				i = 0 ;
	
				EOS = 1 ;
	
				break ;
			}

	
			else if( EOS && REPEAT( timer_value ))
			{
				 key = repeat_key = bit_pattern ;
	
				 break ;
			}
		}
	}

	return ;
}


unsigned char key_map( void )
{

	if( key == repeat_key )
	{
		if( repeat_count == 2 )
		{
			repeat_count = 0 ;
		}
		else
		{
			repeat_count++ ;
		}
	}

	switch( key )
	{
		case 0xBA45FF00 :  if( alphanumeric_flag )
				  		   {
								return '1' ;
				  		   }

				  		   else
				  		   {
				  				if( case_flag )
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
									
									return ( 'A'+ repeat_count ) ;
								}

								else
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}

									return ( 'a' + repeat_count ) ;
								}
				   			}

		case 0xB946FF00 :  if( alphanumeric_flag )
				  		   {
								return '2' ;
				  		   }

				  		   else
				  		   {
				  				if( case_flag )
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
							
									return ( 'D'+ repeat_count ) ;
						   		}

						   		else
						   		{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}

									return ( 'd' + repeat_count ) ;
						   		}
				   			}
		
		case 0xB847FF00 :  if( alphanumeric_flag )
			   			   {
								return '3' ;
				  		   }

				  		   else
				  		   {
				  				if( case_flag )
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
							
									return ( 'G'+ repeat_count ) ;
								}

								else
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}

									return ( 'g' + repeat_count ) ;
								}
				   			}

		case 0xBB44FF00 :  if( alphanumeric_flag )
				  		   {
								return '4' ;
				   		   }

				  		   else
				  		   {
				  				if( case_flag )
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
							
									return ( 'J'+ repeat_count ) ;
								}

								else
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}

									return ( 'j' + repeat_count ) ;
								}
						   }
			
		case 0xBF40FF00 :  if( alphanumeric_flag )
						  {
								return '5' ;
						  }
		
						  else
						  {
						  		if( case_flag )
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
									return ( 'M'+ repeat_count ) ;
								}
		
								else
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
		
									return ( 'm' + repeat_count ) ;
								}
						   }

		case 0xBC43FF00 :  if( alphanumeric_flag )
						  {
								return '6' ;
						  }
		
						  else
						  {
						  		if( case_flag )
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
									return ( 'P'+ repeat_count ) ;
								}
		
								else
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
		
									return ( 'p' + repeat_count ) ;
								}
						   }

		case 0xF807FF00 :  if( alphanumeric_flag )
				  		   {
								return '7' ;
				  		   }

				       	   else
				  		   {
				  				if( case_flag )
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
							
									return ( 'S'+ repeat_count ) ;
								}

								else
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}

									return ( 's' + repeat_count ) ;
								}
				  		   }

		case 0xEA15FF00 :  if( alphanumeric_flag )
				  		   {
								return '8' ;
				  		   }
		
						  else
						  {
						  		if( case_flag )
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
									return ( 'V'+ repeat_count ) ;
								}
		
								else
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
		
									return ( 'v' + repeat_count ) ;
								}
						   }

		case 0xF609FF00 :  if( alphanumeric_flag )
						   {
								return '9' ;
						   }

						  else
						  {
						  		if( case_flag )
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
		
									if( repeat_count == 2 )
									{
										 repeat_count = 0 ;
									}
		
									return ( 'Y'+ repeat_count ) ;
								}
		
								else
								{
									if( repeat_key != 0 )
									{
										CURSOR_LEFT ;
									}
									if( repeat_count == 2 )
									{
										 repeat_count = 0 ;
									}
		
									return ( 'y' + repeat_count ) ;
								}
						   }

		case 0xE619FF00 :  if( alphanumeric_flag )
						   {
								return '0' ;
						   }
		
						  else
						  {
								return ' ' ;
						  }

		case 0xE916FF00 :  if( repeat_key != 0 )
						  {
								CURSOR_LEFT ;
						  }

						  return ( '*'+ repeat_count ) ;
		
		case 0xF20DFF00 :  if( repeat_key != 0 )
						  {
								CURSOR_LEFT ;
						  }
		
						  return ( '#'+ repeat_count ) ;
	}
}