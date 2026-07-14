# CLOs
* CLO9 - Explain the ways Vertex Buffer Objects can improve your display performance

# Introduction

As we saw in our [Get_to_the_Point](../week_2/get_to_the_point.md.md) exploration, we *briefly* took a look at the *Anatomy of a Shader*. We discussed that you can send data *into* a shader (as well as out). In that program, we didn't actually do that as we hardcoded our vertex position directly into the shader code. Let us know, fill in those blanks on sending data to a shader.

# Shader Side

In the *Anatomy of a Shader*, there were two ways to get data into a shader by either using `in` or `uniform`. One is used for data that will change for each vertex, the other for data that we will reuse for multiple vertices. Do you have a guess as to why we make this distinction?

**HIDE ANSWER: It is to be more effecient. While modern GPUs have *gigabytes* of RAM, but we don't want to waste it! We want to use only what we must, so we can process more vertices, which leads to more detailed scenes.**

## Shader Inputs (`in` variables)

For data that is updated for each vertex, we want to use `in`. This includes:

* Position (`vec3`)
* Color (`vec4`)
* Surface Normal (`vec3`) - used for lighting
* Texture Coordinates (`vec2`)

We won't be using all of these right away, I just wanted to give you a look ahead. I am also showing you the most common data type associated with each type of information. When used in the *Vertex Shader*, we will call these variables *Vertex Attributes*, which aligns with how the functions we use to set them (see below). In all other shader types, we will simply call them *Shader Inputs*.

## Uniform Inputs (`uniform` variables)

For data that will be used again-and-again across entire objects (or even frames), we want to use `uniform`. This allows us to load data onto the GPU *once* and reuse it for as many vertices as we want. Remember, at any given time, the GPU pipeline is processing *tens* of thousands (if not *hundreds* of thousands) of vertices all at once. If we were to use `in` for data that doesn't change from vertex-to-vertex, think of all that wasted RAM on the GPU!

THe most common types would include:

* The Model, View, and Projection Matrices (`mat4`)
* Light Positions and Color (`vec3` and `vec4` respectively)
* Camera Position (`vec3`)
* Texture Data (`sampler2D`)

Soon, we will be using *a lot* of matrices in our shaders as we transform our objects and light them. Later, we will be covering lighting, camera movment, and appyling textures to objects.

# Application Side

Just as there are two ways for shaders to receive input data (`in` and `uniform`), there are two ways to send that data to the shader from our OpenGL application:

* Using a buffer
* Sending it directly

## Buffers

Let's get ready to Rumble![^1] It's time![^2]...to talk about *buffers*. When a buffer is not doing ring announceing[^3], they are a way of storing data that we need to send into the pipeline. OpenGL has a way of declaringe a "blob" of memory, that we can then fill however we like.

OpenGL offers at least four types of buffer objects, but we are going to focus on just one here: `GL_ARRAY_BUFFER`. This will hold our vertex data that will be sent to the pipeline (one after the other).

You may be wondering, "Why didn't we use one of these when we rendered our very first vertex?" That's a great question! It is because we never actually sent data to the vertex shader; we hardcoded it. If we want to define our vertices in our application and send them to the shader, we *must* use something called a *Vertex Buffer Object* (VBO).

Let's look at how we go about this. I am going to present all the code chunks all at once so you can see everything in context. See if you can deduce what all of it does, before moving onto the explaination.

Global declarations:

```C++
// Vertex data for a simple triangle
float vertices[] = {
      -2.0f, -2.0f, 0.0f,
       2.0f, -2.0f, 0.0f,
       0.0f,  2.0f, 0.0f
};

#define numVAOs 1
#define numVBOs 1

GLuint vao[numVAOs];
GLuint vbo[numVBOs];
```

Inside `init()`:

```C++
// Generate and bind VAO
glGenVertexArrays(numVAOs, vao);
glBindVertexArray(vao[0]);

// Generate and bind VBO
glGenBuffers(numVBOs, vbo);
glBindBuffers(GL_ARRAY_BUFFER, vbo[0]);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// Set position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), 0);
glEnableVertexAttribArray(0);

// unbind VAO
glBindVertexArray(0);
```

Inside `display()`:

```C++
glUseProgram(renderingProgram); //defined and built elsewhere
glBindVertexArray(vao[0]);
glDrawArrays(GL_TRIANGLES, 0, 3);
glBindVertexArray(0);
```

## Setting a Uniform

[^1]: This is a [Michael Buffer](https://en.wikipedia.org/wiki/Michael_Buffer) reference. If you don't get it, it means I am finally beyond old and should stop trying to make references. HA! Fat chance I will do that!
[^2]: This was a [*Bruce Buffer*](https://en.wikipedia.org/wiki/Bruce_Buffer) reference; I couldn't mention one half-brother without mentioning the other!
[^3]: Now I am just seeing how many times I can beat this dead horse. If yo don't know what I mean, see footnotes 1 and 2. If you manged to read footnote 3 without reading those two first, this joke makes no sense, but neither does your clicking on this footnote and not the others!

# IDEAS
* SHow how to bind colors so we can alternate the point colors