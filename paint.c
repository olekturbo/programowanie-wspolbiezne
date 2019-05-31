#define _REENTRANT
#include <X11/Xlib.h>
#include <X11/X.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

Display *mydisplay;
Window mywindow;
XSetWindowAttributes mywindowattributes;
XGCValues mygcvalues;
GC mygc;
Visual *myvisual;
int mydepth;
int myscreen;
Colormap mycolormap;
XColor colors[10],dummy;
XEvent myevent;
pthread_t tid,tid1;
struct buffer {int x;int y;int action;int prev; int color;};
int bufsize;
int p;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
int toDo;
int currentColor;
int tmpColor, tmpAction;
int tmpX, tmpY;

void end()

{
pthread_mutex_destroy(&lock);
}

void *reader(void *argum)

{
   struct buffer *buf;
   int xr,yr,xr1,yr1;
   int fd;

   buf=(struct buffer *) malloc(bufsize);
   while (!(fd=open("rysunek",O_RDONLY,0700)));
   while (1)
   {  
	   
      if (read(fd,buf,bufsize)>0)
      {
		  if(buf->color!=NULL)
		  {
			  tmpColor = buf->color;
		  }
		  else
		  {
			  tmpColor = currentColor;
		  }
		  
		  if(buf->action!=NULL)
		  {
			  tmpAction = buf->action;
		  }
		  else
		  {
			  tmpAction = toDo;
		  }
		  
		 
		 XSetForeground(mydisplay,mygc,colors[tmpColor].pixel);
		 
            if(buf->x > 120) {
				pthread_mutex_lock(&lock);
            if (buf->prev==0)
            {
               xr1=buf->x;
               yr1=buf->y;
			   tmpX = xr1;
			   tmpY = yr1;
			   
            }
            else  if (buf->prev==1)
            {
               xr=buf->x;
               yr=buf->y;
               if(tmpAction == 0 || tmpAction == 3) 
               {
				   XFillRectangle(mydisplay, mywindow, mygc, xr - 3, yr - 3, 3, 3);
			   }
               XFlush(mydisplay);
               xr1=xr;
               yr1=yr;
            }
            else if (buf->prev==2)
            {
			   xr=buf->x;
               yr=buf->y;
			   if(tmpAction == 2)
               {
				   if (tmpX >= xr)
					{
					if (tmpY >= yr)
					{
						XFillRectangle(mydisplay, mywindow, mygc, xr, yr, abs(xr - tmpX), abs(yr - tmpY));
					}
					else if (tmpY < yr)
					{
						XFillRectangle(mydisplay, mywindow, mygc, xr, tmpY, abs(xr - tmpX), abs(yr - tmpY));
					}
				}
				else if (tmpX < xr)
				{
					if (tmpY >= yr)
					{
						XFillRectangle(mydisplay, mywindow, mygc, tmpX, yr, abs(xr - tmpX), abs(yr - tmpY));
					}
					else if (tmpY < yr)
					{
						XFillRectangle(mydisplay, mywindow, mygc, tmpX, tmpY, abs(xr - tmpX), abs(yr - tmpY));
					}
				}
			   }
			   if(tmpAction == 1)
			   {
				    XPoint points[4];
				    points[0].x = tmpX - 5;
					points[0].y = tmpY - 5; 
					points[1].x = tmpX + 5;
					points[1].y = tmpY + 5;
					points[2].x = xr + 5;
					points[2].y = yr + 5;
					points[3].x = xr - 5;
					points[3].y = yr - 5;
					XFillPolygon(mydisplay, mywindow, mygc, points, 4, Complex, CoordModeOrigin);
			   }
			   if(tmpAction == 4)
			   {
				    if (tmpX >= xr)
            {
                if (tmpY >= yr)
                {
                    XDrawArc(mydisplay, mywindow, mygc, xr, yr, abs(xr - tmpX), abs(yr - tmpY), 360*64, 360*64);
                }
                else if (tmpY < yr)
                {
                    XDrawArc(mydisplay, mywindow, mygc, xr, tmpY, abs(xr - tmpX), abs(yr - tmpY), 360*64, 360*64);
                }
            }
            else if (tmpX < xr)
            {
                if (tmpY >= yr)
                {
                    XDrawArc(mydisplay, mywindow, mygc, tmpX, yr, abs(xr - tmpX), abs(yr - tmpY), 360*64, 360*64);
                }
                else if (tmpY < yr)
                {
                    XDrawArc(mydisplay, mywindow, mygc, tmpX, tmpY, abs(xr - tmpX), abs(yr - tmpY), 360*64, 360*64);
                }
            }
			   }
			   XFlush(mydisplay);
			}
            pthread_mutex_unlock(&lock);
			}
         
      }
   }
}      

main()

{
   int xw,yw,xw1,yw1;
   int fdw;
   struct buffer *bufw;
   
   bufsize=sizeof(struct buffer);
   bufw=(struct buffer *) malloc(bufsize);

   XInitThreads();
   mydisplay = XOpenDisplay("");
   myscreen = DefaultScreen(mydisplay);
   myvisual = DefaultVisual(mydisplay,myscreen);
   mydepth = DefaultDepth(mydisplay,myscreen);
   mywindowattributes.background_pixel = XWhitePixel(mydisplay,myscreen);
   mywindowattributes.override_redirect = False;
   mywindowattributes.backing_store = Always;
   mywindowattributes.bit_gravity = NorthWestGravity;
   
   mywindow = XCreateWindow(mydisplay,XRootWindow(mydisplay,myscreen),
                            0,0,500,500,10,mydepth,InputOutput,
                            myvisual,CWBackingStore|CWBackingPlanes|CWBitGravity|
                            CWBackPixel|CWOverrideRedirect,&mywindowattributes);

   XSelectInput(mydisplay,mywindow,KeyPressMask|ButtonPressMask|ButtonReleaseMask|ButtonMotionMask|ExposureMask);
                    
   mycolormap = DefaultColormap(mydisplay,myscreen);                 
                            
   XAllocNamedColor(mydisplay,mycolormap,"gray",&colors[0],&dummy);
   XAllocNamedColor(mydisplay,mycolormap,"blue",&colors[1],&dummy);
   XAllocNamedColor(mydisplay,mycolormap,"red",&colors[2],&dummy);
   XAllocNamedColor(mydisplay,mycolormap,"green",&colors[3],&dummy);
   XAllocNamedColor(mydisplay,mycolormap,"black",&colors[4],&dummy);
   XAllocNamedColor(mydisplay,mycolormap,"yellow",&colors[5],&dummy);
   XAllocNamedColor(mydisplay,mycolormap,"brown",&colors[6],&dummy);
   XAllocNamedColor(mydisplay,mycolormap,"orange",&colors[7],&dummy);
   XAllocNamedColor(mydisplay,mycolormap,"white",&colors[8],&dummy);
   XAllocNamedColor(mydisplay,mycolormap,"aqua",&colors[9],&dummy);
   
   XMapWindow(mydisplay,mywindow);
   
   mygc = DefaultGC(mydisplay,myscreen);

   fdw=open("rysunek",O_WRONLY|O_CREAT|O_APPEND,0700);
   pthread_create(&tid,NULL,reader,(void *) &p);
   pthread_create(&tid1,NULL,reader,NULL);

   while (1)
   
   {
     
      XNextEvent(mydisplay,&myevent);
      switch (myevent.type)
      
      {
		  
		 case Expose:
		 //menu
               
               XSetForeground(mydisplay,mygc,colors[9].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 0, 0, 120, 500);
	   
			   XSetForeground(mydisplay,mygc,colors[0].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 0, 0, 50, 30);
			   
			   XSetForeground(mydisplay,mygc,colors[0].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 0, 35, 50, 30);
			   
			   XSetForeground(mydisplay,mygc,colors[0].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 0, 70, 70, 30);
			   
			   XSetForeground(mydisplay,mygc,colors[0].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 0, 105, 50, 30);
			   
			   XSetForeground(mydisplay,mygc,colors[0].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 0, 140, 50, 30);
			   
			   XSetForeground(mydisplay,mygc,colors[0].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 0, 230, 20, 20);
			   
			   XSetForeground(mydisplay,mygc,colors[4].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 0, 260, 20, 20);
			   
			   XSetForeground(mydisplay,mygc,colors[1].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 30, 230, 20, 20);
			   
			   XSetForeground(mydisplay,mygc,colors[5].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 30, 260, 20, 20);
			   
			   XSetForeground(mydisplay,mygc,colors[2].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 60, 230, 20, 20);
			   
			   XSetForeground(mydisplay,mygc,colors[6].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 60, 260, 20, 20);
			   
			   XSetForeground(mydisplay,mygc,colors[3].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 90, 230, 20, 20);
			   
			   XSetForeground(mydisplay,mygc,colors[7].pixel);
			   XFillRectangle(mydisplay, mywindow, mygc, 90, 260, 20, 20);

			   // menu cd
			   XSetForeground(mydisplay,mygc,colors[2].pixel);
			   XDrawString(mydisplay, mywindow, mygc, 10, 20, "Pedzel", 6);
			   
			   XSetForeground(mydisplay,mygc,colors[2].pixel);
			   XDrawString(mydisplay, mywindow, mygc, 10, 55, "Linia", 5);
			   
			   XSetForeground(mydisplay,mygc,colors[2].pixel);
			   XDrawString(mydisplay, mywindow, mygc, 10, 90, "Prostokat", 9);
			   
			   XSetForeground(mydisplay,mygc,colors[2].pixel);
			   XDrawString(mydisplay, mywindow, mygc, 10, 125, "Gumka", 5);
			   
			   XSetForeground(mydisplay,mygc,colors[2].pixel);
			   XDrawString(mydisplay, mywindow, mygc, 10, 160, "Elipsa", 6);
			   
			   XDrawString(mydisplay, mywindow, mygc, 10, 200, "Kolory", 6);
			 break;
         
         case ButtonPress:

              xw1=myevent.xbutton.x;
              
              yw1=myevent.xbutton.y;

   
              bufw->x=xw1;
              bufw->y=yw1;
              bufw->prev=0;
              
                       //akcje
		     if(bufw->x >= 0 && bufw->x <= 50 && bufw->y >=0 && bufw->y <= 30) {
				 toDo = 0;
			 }
			 
			 if(bufw->x >= 0 && bufw->x <= 50 && bufw->y >=35 && bufw->y <= 65) {
				 toDo = 1;
			 }
			 
			 if(bufw->x >= 0 && bufw->x <= 70 && bufw->y >=70 && bufw->y <= 100) {
				 toDo = 2;
			 }
			 
			 if(bufw->x >= 0 && bufw->x <= 50 && bufw->y >=105 && bufw->y <= 135) {
				 toDo = 3;
			 }
			 
			 if(bufw->x >= 0 && bufw->x <= 50 && bufw->y >=140 && bufw->y <= 170) {
				 toDo = 4;
			 }
			 
			 if(toDo == 3)
			 {
				 currentColor = 8;
			 }
              
              		 //kolory
			 if(bufw->x >= 0 && bufw->x <= 20 && bufw->y >=230 && bufw->y <= 250) {
				 currentColor = 0;
			 }
			 
			 if(bufw->x >= 0 && bufw->x <= 20 && bufw->y >=260 && bufw->y <= 290) {
				 currentColor = 4;
			 }
			 
			 if(bufw->x >= 30 && bufw->x <= 50 && bufw->y >=230 && bufw->y <= 250) {
				 currentColor = 1;
			 }
			 
			 if(bufw->x >= 30 && bufw->x <= 50 && bufw->y >=260 && bufw->y <= 290) {
				 currentColor = 5;
			 }
			 
			 if(bufw->x >= 60 && bufw->x <= 80 && bufw->y >=230 && bufw->y <= 250) {
				 currentColor = 2;
			 }
			 
			 if(bufw->x >= 60 && bufw->x <= 80 && bufw->y >=260 && bufw->y <= 290) {
				 currentColor = 6;
			 }
			 
			 if(bufw->x >= 90 && bufw->x <= 110 && bufw->y >=230 && bufw->y <= 250) {
				currentColor = 3;
			 }
			 
			  if(bufw->x >= 90 && bufw->x <= 110 && bufw->y >=260 && bufw->y <= 290) {
				currentColor = 7;
			 }
			 
			  bufw->action=toDo;
			  bufw->color=currentColor;
              
              write(fdw,bufw,bufsize);
              break;

         case MotionNotify:
         
              xw=myevent.xmotion.x;
              
              yw=myevent.xmotion.y;


              bufw->x=xw;
              bufw->y=yw;
              bufw->prev=1;
              write(fdw,bufw,bufsize);

              xw1=xw;
              
              yw1=yw;
              
              break;

         case KeyPress:
          
              XCloseDisplay(mydisplay);

              end();   
              close(fdw);
              exit(0); 
              
	     case ButtonRelease:
				 xw1=myevent.xbutton.x;
              
              yw1=myevent.xbutton.y;
   
   
              bufw->x=xw1;
              bufw->y=yw1;
              bufw->prev=2;
              write(fdw,bufw,bufsize);
              break;
              
      }
      
  }
              
}
                                             
