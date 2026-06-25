# CLOs
* CLO10 - Create and use an OpenGL vertex and fragment shader

# Introduction

So, we have been teasing you a bit about actually getting into OpenGL programming. We exhaustively have covered the layout and function of our starter program, which we used to test our tool installation. But we didn't *draw* anything; we just cleared the window with a specified color.

That all changes *now*! We are finally ready to *Get to the Point!* (editor's note: would you believe I loathe puns?). We are going to display our very first PIXEL!

# Setting Up

It is a good idea to follow along with all the examples in the course materials as we go. Therefore, we highly recommend that you create a new project for each exploration/example. Don't just continually write over the one start project; that path leads only to frustration.

Go ahead and use the template we set up in an earlier exploration to create a new OpenGL project. You can name it whatever you want, but I personally like to name it the same name as the "chapter" I am working on. In this case, "Get to the Point".

# Shader Code

While OpenGL, has ways of drawing without using *shaders*, in this course we are focusing 100% on the *programmable pipeline*. Therefore, we first need to write some quick shader code.

For now, we are going to keep our shader code *very* simple. Do you remember which shaders are required for every OpenGL application?

**HIDE ANSWER: The Vertex and Fragment Shaders!**

Shader code can be directly written into the `.cpp` file, but that is a bad habit to develop, so we are going to jump ahead to using separate files to hold our shader source code.

## Anatomy of a Shader

Shader source code uses GLSL (OpenGL Shader Language), which follows most of the same syntax requirements a C/C++. Let's look at a *generic* shader.

```
#version 410 core

in type in_variable_name;
out type out_variable name;

uniform type uniform_name;

// optional helper functions

void main() {
    // shader logic
}
```
The very first line of every shader program *must* specify the version of OpenGL targeted. For us, we will be focusing exclusively on OpenGL 4.1, so all of our shader programs will start with `#version 410 core`. The `core` keyword basically forces using the fully programmable pipeline by removing all the deprecated fixed-function methods.

Next, we may need to specify the information being passed into and out of the shader program. We do this with the `in` and `out` keywords. The "type" will change depending on what is required, but will often be some form of vector (`vec3` or `vec4`). Remember, OpenGL is a *pipeline* so the inputs come from earlier in the pipeline and the outputs are fed into the next portion of the pipeline.

**Nota bene:** There are some built-in `in` and `out` variables, such as `gl_Position`. A vertex shader must set this value before ending so the rasterizer knows where the vertex is in order to start building primitives.

After the `in` and `out` variables, we can specify any `uniform` variables we may need. Unlike the previous variables, *uniform variables* are specified at the application level and remain constant across all vertices/fragments during a given draw. We will often use these for keeping track of time for animations or even textures.

Each shader file will need to have a `main` function, but you can also write other functions as needed (just like in any C/C++ program). This is where you use the values passed in (if any) and specify the values passed out (if any).

## Our First Vertex Shader!

We are going to keep things simple for now and create the most basic vertex shader imaginable.

First, create a new file in your project and call it "shader.vert" In that file, paste the following:

```
#version 410 core
void main() {
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
```
As we mentioned above, every vertex shader needs to specify a `gl_Position` before calling it quits. In this example, we are specifying that our vertex is at the origin, where X, Y, and Z all equal 0.0. 

If we can specify any point in a 3D world using only three values (X, Y, and Z), why do you we need a fourth value? That is a good question! It is for *convenience* (mostly). Later, we will see why having a `vec4` instead of a `vec3` is very helpful when it comes to matrix multiplcation. For know, just trust the process.

That's it! That is the entire vertex shader! Easy, right!?

## Our First Fragment Shader!

Again, we are going to keep things simple for now. Create a new file and name it "shader.frag" In that file, paste the following:

```
#version 410 core
out vec4 color;
void main() {
    color = vec4(0.0, 0.0, 1.0, 1.0);
}
```
