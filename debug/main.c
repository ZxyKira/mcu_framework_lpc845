/*****************************************************************************
 *			File Name		 : main.c
 *			Version			 : V1.0
 *			Create Date	 : 2020-10-06
 *      Modufy Date  : 2020-10-06
 *      Author       : Danny
 *			Information  :
 */
 
/*****************************************************************************
 *	Extern Function/Variable
 */
extern void start(void);
extern void loop(void);


/*****************************************************************************
 *	Public function
 */
int main(){
	start();
	while(1)
		loop();
}

/*****************************************************************************
 *	End of file
 */
