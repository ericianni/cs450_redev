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

Let's get ready to Rumble![^1] It's time![^2]...to talk about *buffers*. When a buffer is not doing ring announceing[^3], they act as a way of storing data that we need to send into the pipeline. OpenGL has a way of declaringe a "blob" of memory, that we can then fill however we like.

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
// Generate VAOs and VBOs
glGenVertexArrays(numVAOs, vao);
glGenBuffers(numVBOs, vbo);

// Bind VAOs and VBOs
glBindVertexArray(vao[0]);
glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

// Fill VBO with vertex data
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// Set position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
glEnableVertexAttribArray(0);

// unbind VAO and VBO
glBindVertexArray(0);
glBindBuffer(GL_ARRAY_BUFFER, 0);
```

Inside `display()`:

```C++
glUseProgram(renderingProgram); //defined and built elsewhere
glBindVertexArray(vao[0]);
glDrawArrays(GL_TRIANGLES, 0, 3);
glBindVertexArray(0);
```

Let's take this piece by piece. The first thing we do is declare our "globals." For this example, we are declaring our vertices in our code, but soon we will learn how to store this information in `.obj` files. In the `vertices` array we have nine `floats`. Each set of three numbers represents a vertex.

Next, we `#define` two *macros*, which can be thought of in the same way as any other constant variable. This isn't a C++ course, so we will skip the explanation as to *why* this is required. The two macros we define will represent the number of `Vertex Array Objects` and `Vertex Buffer Objects` we want to create.

As we mentioned in an earlier exploration, OpenGL requires that we bind at least one VAO (even if it is "empty") before we can call `glDrawArrays`. The VAO is where we will store the layout of how we have configured our vertex data (details to follow).

As mentioned above, we need at least on VBO to send vertex data to the vertex shader. For now, we are just going to use one VBO (to hold position data), but it is not uncommon to have four, though there really is no limit. Each VBO would hold different vertex data (see *Shader Inputs* above for details).

We then use these two macros to declare an array to hold our soon-to-be created VAOs and VBOs. The data type for these arrays must be `GLuint`. Take a guess as to why that is.

**HIDE ANSWER: As we have seen before, a common use of `GLuint` variables is to hold the ID numbers used by OpenGL to reference all kinds of things.**

Now we move onto the code in `init()`. We only need to generate our arrays and buffers *once*, so doing it in `init()` makes perfect sense. We need to allocate the memory for our VAOs and VBOs.

We start by calling `glGenVertexArrarys(numVAOs, vao)`. We pass in the number of VAOs we want to create (`numVAOs`) and the array we wish to fill with the VAO ID numbers (`vao`).

We do something similar to allocate the memory needed for our VBOs using `glGenBuffers(numVBOs, vbo)`. Again, this creates the buffers and fills `vbo` with the ID numbers associated with each newly created buffer.

**Editors Note:** You may see these calls split up, with `glBindVertexArray` in between, which is *technically* allowed. That said, it is better to get into the habit of calling all your `glGen` calls all at the same time so you don't forget and make mistakes.

Next, we need to tell OpenGL *which* VAO we wish to modify by *binding* it (`glBindVertexArray(vao[0])`). It is absolutely critical that we call this before we bind our VBOs. If we do it in the other order, OpenGL will not associate the changes to the VBO with the desired VAO. 

Once that is done, we can call `glBindBuffer(GL_ARRAY_BUFFER, vbo[0])`, which informs OpenGL which VBO we will be modifying. During the binding, we have to tell OpenGL the type of array we wish to use. This is important, because when we generate the buffers, they are simple "blobs" of memory, with no real structure. `GL_ARRAY_BUFFER` tells OpenGL to treat the data as an array (almost always consisting of vertex data).

Quick recap, we first generated our VAOs/VBOs. We then *bound* both of them (first the VAO, then the VBO). Now, it is time to *fill* the VBO. We do this using `glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW)`. Let's break down each of these parameters.

* `GL_ARRAY_BUFFER` - tells OpenGL that we are filling this is sequential indexed data
* `sizeof(vertices)` - tells OpenGL that the amount of memory we need is enough to hold all our vertex data
* `vertices` - tells OpenGL where to find the data to load into the buffer
* `GL_STATIC_DRAW` - tells OpenGL how often you plan on updating this buffer and how often you plan on accessing it.[^4] This helps the GPU to know where in memory to most effeciently store the buffer data. `STATIC` means it will be written once and accessed many times. `DRAW` means the CPU will write the data and the GPU will read it.

Now that we have loaded our buffer, we need to inform OpenGL how we intend to get that data into our vertex shader. Since this data will change for every vertex, we must use *Vertex Attributes* (not *Uniforms*). We do this using `glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0)`. Let's break down each parameter.

* `0` - tells OpenGL which *position* within the VAO we want to store the buffer. As mentioned earlier, the first position (`0`) is typically associated with the position data for the vertices
* `3` - tells OpenGL how many elements of the buffer we want to send to the shader at a time. Since we are dealing with position coordinates (x, y, and z), we want to send three
* `GL_FLOAT` - tells OpenGL the type of data each element is; vertices are stored as floats
* `GL_FALSE` - tells OpenGL that we don't want the data normalized (i.e. converted to [-1.0, 1.0]) during the transfer
* `3 * sizeof(float)` - tells OpenGL how much "space" or *stride* there is between the beginning of each element. Here, we are telling OpenGL that a new vertex starts after every three floats. In our example, our vertex data is *tightly packed*, meaning that each vertex consists only of the three position elements. Many times you will see *interleaved* data where each vertex has not only position stored, but also color, texture coordinates, and normals. In those instances, you have to change the multiplication factor so OpenGL knows where the next vertex starts in the buffer.[^5] In this course, we will only be using tightly packed buffers to keep things easier to read.
* `0` - tells OpenGL which index in the array is the start of the data we want to use. In this course, our buffers will only ever hold one object at a time, so we will only ever start at `0`

That's it! Now our VAO contains a *vertex attribute* at position 0 that contains our vertex data. We will now be able to quickly load this VAO and restore all these "settings" whenever we want to draw our stored object. 

But, before we go, we want to make sure we clean up after ourselves by *unbinding* both our VAO and VBO. We have to unbind the VAO first using `glBindVertexArray(0)`. Using `0` instead of a VAO ID automatically unbinds the current VAO. Simularly, we unbind the current VBO using `glBindBuffer(GL_ARRAY_BUFFER, 0)`. Not doing these two steps can cause bugs that are very tricky to track down, so make sure you do it and in the correct order!

Whew! That was a bunch to take in, but we aren't done yet! Now, we actually need to *use* the buffer data by sending it to the shader. As we did when we drew our first point (see ![Get to the Point](../week_2/get_to_the_point.md)), we have to load our shader program: `glUseProgram(renderingProgram)`.

Now we can load our desired VAO to pass to the shader(s): `glBindVertexArray(vao[0])`. Again, *binding* just tells OpenGL what context we wish to use for the next function calls. Since we already filled the VAO in `init()`, we are all ready to call `glDrawArrays(GL_TRIANGLES, 0, 3)`. Instead of me *telling* you what each of these things do, why don't you guess?

**HIDE ANSWER:**
* `GL_TRIANGLES` - defines the *mode* to use when drawing the vertices. Here, we are telling the shader to group every three vertices together into a triangle
* `0` - specifies the starting index of our vertex data. In this course, our buffers will only contain one object so we will always start at `0`
* `3` - specifies the number vertices we will be reading in
**END HIDING**

Now that our buffer data has been sent off to the vertex shader, we must once again clean up after ourselves by calling `glBindVertexArray(0)`.

Congratulations, you have just learned how to pass *vertex attributes* to a vertex shader! As mentioned above, this is just one way of getting data into a shader. The other way requires using *uniforms*, which we are going to cover next.

## Setting a Uniform

CONTENT YET TO BE WRITTEN

[^1]: This is a [Michael Buffer](https://en.wikipedia.org/wiki/Michael_Buffer) reference. If you don't get it, it means I am finally beyond old and should stop trying to make references. HA! Fat chance I will do that!
[^2]: This was a [*Bruce Buffer*](https://en.wikipedia.org/wiki/Bruce_Buffer) reference; I couldn't mention one half-brother without mentioning the other!
[^3]: Now I am just seeing how many times I can beat this dead horse. If yo don't know what I mean, see footnotes 1 and 2. If you manged to read footnote 3 without reading those two first, this joke makes no sense, but neither does your clicking on this footnote and not the others!
[^4]: [Full breakdown of `glBUfferData` usage types](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml)
[^5]: Techincally, with tightly packed data, you can just use `0` instead of using `X * sizeof(type)` and OpenGL will automatically calculate the stride, but I prefer to be explicit in all things.

# IDEAS
* SHow how to bind colors so we can alternate the point colors