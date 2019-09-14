# Learn_Game
# Thanks to Joey de vries for the wonderfull tutorial which helped me to learn graphics promaramming.
https://joeydevries.com/#home
https://learnopengl.com


Created an 2D Game in OpenGL. all the required library will be available in this project its self.

Setting Up the Project
* Open the project in visual studio.
* Mostly you wont get any linking error if your getting so follow this steps.
* Check the soution configuration is set to DEBUG.
* Check the solution platform is x86.

If still getting the linking error then we have to relink the project.
* Go to Solution Explorer window -> Select your project (Learn_Opengl)-> Right Click -> Properties.
* Configuration is set to Debug and platform is Win32.
* Configuration Properties-> VC++ Directories -> General -> Include Directories -> Edit opiton in drop down box.
* Include Directories window will pop up -> Click on New Line.
* Write the following path in it.
* $(SoluionDir)deps\include\
* Select the OK button.
* Configuration Properties-> VC++ Directories -> General -> Library Directories -> Edit opiton in drop down box.
* Library Directories window will pop up -> Click on New Line.
* Write the following path in it.
* $(SoluionDir)deps\lib\
* Select the OK button.

Last setp is we have to add additional dependencies
* Configuration Properties -> Linker -> Input -> Additional dependencies -> Edit opiton in drop down box.
* Additional dependencies window will be popup in that add the follewing name
* Opengl32.lib, glew32.lib, glfw3.lib, soil.lib.
* Click OK -> Apply - > Ok.

we have linked the project so build and chceck for any linker error any error kindly send me the screenshot to this email id sathish_ravi62@yahoo.com
* Now run the program and you make get the folowing error glew32.dll was not found. click OK.
* Download the library from the given link. (https://sourceforge.net/projects/glew/files/latest/download).
* Extract it and open the glew-2.1.0-win32 folder->glew-2.1.0->bin->Relese->Win32-> Copy the glew32.dll file.
* Go to the Soluion Explorer window right clicking on the solution file -> Open Folder in File explorer.
* It will open the project in the file explorer-> open the Debug folder plast the file.

If you run the program will start.
GO and explore the project and see what is happening.




