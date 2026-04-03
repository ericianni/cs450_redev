# Introductcion
Now that we have gone over the history computer graphics, it is time to get our hands dirty! Before we can put pixels to screen, we need to ensure our development environment is setup correctly. There are lots of ways of using OpenGL: C++, Python, and Java. In this course, we are going to focus on writing applications in C++.

We will be using the following tech stack:
* Visual Studio
* GLFW (Graphics Library Framework)
* GLEW (OpenGL Extension Wrangler Library)
* GLM (OpenGL Mathematics)
* SOIL 2 (a fork of Simple OpenGL Image Library)
* Premake (needed to compile SOIL 2)

Over the course of this exploration, we will walk you through how to install all of these components. At the end you will be able to display a single pixel to the screen!

You are free to make changes to where things are installed, but the choices made for this exploration are designed to make things easy to get working. If you deviate from these instructions you may run into difficulties, which we may not be able to help you fix. Therefore, only strike your own path if you are experienced and capable of troubleshooting.

Warning: Many of the components discussed below are in constant flux, so sometimes version numbers won't line up exactly. If that happens, just reach out to the instructor or look to Ed Discussions for solutions. I will endeavour to keep this page up-to-date, but things *will* slip through the cracks.

# Visual Studio Installation

Most of you probably already use VS Code, where VS stands for "Visual Studio." Unfortunately, VS Code is primarily a text editor; for OpenGL we need a full-fledged Integrated Development Environment (IDE). This is a beefy piece of software, so make sure you have the hard drive space before proceeding; some installs can take 20GB or more.

At the time of writing, the most recent version of Visual Studio is 2026, which we will be using. If you have an older version of Visual Studio, you will most likely not need to upgrade, as OpenGL doesn't really care. You may just need to "retarget" the sample project to your version.

The first step is to go to Azure for Students (https://azure.microsoft.com/en-us/free/students). Sign in either by clicking the "Sign in" button in the top right or by clicking the "Start free" on the landing page. As an OSU student, you should already have a Microsoft Account associated with your OSU email. If you don't already see your email listed, just pick "Use another account" and follow the prompts with your OSU email.

Once you have logged in, you will be taken to the Education Overview page. Find the "Free Software" portion of the dashboard and click "Explore All." This will take you to a screen with a search field into which, you should type "Visual Studio."

You will see multiple versions of Visual Studio and it is very important that you pick the correct one, so please be careful. Click the "Visual Studio Enterprise 2026" option to reveal a sidebar with an option to "View Key" and "Download." 

We will need this key in order to actually run the software, so go ahead and reveal it. I recommend you copy this key into a text file that you can save somewhere safe on your computer. Don't worry, it isn't sensitive like a password, so there is no need to worry about it being saved as plain text.

Once you save your key, go ahead and click the download button. This will prompt you to download the VisualStudioSetup.exe file. This is a fairly small file, and only acts as the installer. After the brief download, go ahead and run the installer.

When the installer is up and running, you will be presented with a dizzing array of options for packages to install. For this course, we only care about "Desktop development with C++." Check this option and click "Install." Unless you have particular needs, my recommendation is to follow all the default installation recommendations. 

Now, sit back and *wait*. This is a huge install and it will take a long time, no matter how fast your internet.

# GLFW

While the VS behemoth downloads, we can use our time to get the rest of the frameworks we will need. Let's start with GLFW.

OpenGL handles a lot for us, but since it is cross platform compatable we need to have platform specific code for things like creating windows and utilizing input devices. GLFW provides the C++ code needed for these crucial tasks.

We need a place to store all these frameworks and I highly recommend that you create a a folder called "GL" on your C drive (or wherever you will be working). It is a really bad idea to install these things on a virtual drive; trust me.

Go to the GLFW homepage (https://www.glfw.org/) and click the "Download" tab at the top. I am going to assume you have a 64-bit computer, so click the button "64-bit Windows binaries" to initiate the download. This should be fairly quick, and once download extract the contents into the GL folder you created.

You should now have a folder in your GL directory called something like "glfw-3.4.bin.WIN64", which you need to rename to simply "GLFW". This is important for later steps, so please don't use any other name.

# GLEW

I am pretty sure VS is still downloading, so let's grab some pieces we need. Now, we are going to aquire GLEW. This is a very technical library that basically preloads the function points from the graphics drivers so we can actually use them in our applications.

Go to the GLEW website (https://glew.sourceforge.net/) and click the "Windows 32-bit and 64-bit" binaries link. Again, extract the contents into the GL folder. Rename this new folder to "GLEW".

# GLM

Before the programmable pipeline, OpenGL relied on a lot of built-in math functions to handle things like rotations. Now, I am not a monster and I am not going to make you write your own matrix math functions; we are going to use GLM, which contains all the graphics maths we will need.

Navigate to the repo containing GLM (https://github.com/g-truc/glm). To download, click the green "<> Code" button and select "Download ZIP". I bet you know what I am going to say now! Extract the file into the GL folder and rename it "GLM".

# SOIL 2

Just like we needed GLFW to manage system calls, we need SOIL 2 to make loading textures easier. 

Go to the SOIL 2 repo (https://github.com/SpartanJ/SOIL2), and just like with GLM, download all the code as a zip. Again, unzip the contents into the GL folder and rename the new directory to "SOIL2".

We need to do a few more things to get SOIL 2 ready to use: download Premake.

# Premake

Premake is similar to *make* in that it is used to build source code. Go to the download page (https://premake.github.io/download/) and download the Windows Pre-Built Binary. It will download as a zip containing one file. Extract this file wherever you like and then copy it into the SOIL2 folder.

Now it is time to dig out your favorite terminal application and navigate to the SOIL2 folder. Once there, you need to type the following `premake5.exe vs2022`. I hear you shouting, "But we downloaded VS 2026, not VS 2022!" and you would be correct. Unfortunately, `vs2026` is not an option at the time of this writing.

Once you have run this command, a new folder will appear in your SOIL2 directory: "make". Click through and then into the "windows" folder contained therein. Now, hopefully, VS 2026 has finished installing, but if it hasn't, we will need to wait. We need VS 2026 to open the "SOIL2.sln" project file. 

When you are able, open the "SOIL2.sln" file, which should open VS 2026. You will likely be shown a screen stating that "Some projects are missing components or targeting outdated toolsets..." Click "Retarget all" and then "Apply." This is necessary to convert the VS2022 settings to the new VS2026 IDE we are using.

On the right, you will see the Solution Explorer. Right-click on the "soil2-static-lib" project and select "Build." Go ahead and close VS 2026, we are done with this project.

