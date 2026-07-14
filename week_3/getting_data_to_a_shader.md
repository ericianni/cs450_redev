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


# Application Side

# IDEAS
* SHow how to bind colors so we can alternate the point colors