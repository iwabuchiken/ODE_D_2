=========================================================================
                OpenGL & Win32 (Tutorial/Example No.24 - ODE Physics)
=========================================================================
Copyright © 31/01/2006, Alan Baylis, All Rights Reserved.


This example program demonstrates how to make a Win32 & OpenGL application that includes the ODE physics library. The program was compiled using Dev-C++ 4.9.5.0 and there is a Visual C++ 7.0 version also available from my website at www.alsprogrammingresource.com.

This code may be freely modified and redistributed but I make no warrantees about it; use at your own risk. If you use this code then give me credit and a link back to this site.

The source code for ODE was released under the GNU Lesser General Public License http://www.opensource.org/licenses/lgpl-license.html or the BSD-style license http://opende.sourceforge.net/ode-license.html and you can download the complete source code and binaries for ODE at the official ode webite at http://www.q12.org. Thanks go to Russell Smith for making ODE an open source project.

Instructions:
Pressing the spacebar resets the geom object back its default position with a random rotation.

The mouse controls the cameras orientation: 
  moving the mouse up and down controls pitch.
  moving the mouse left and right controls yaw.
  pressing the left and right mouse buttons controls roll.

To move the cameras position use the arrow keys and the pgup and pgdown keys.

The numbers 1 - 9 adjust the current cameras movement step amount. 
