# CLOs
* CLO 2 - Describe the names and functions of the elements of the graphics pipeline, as well as the input and output of each stage.

# Introduction

Think back to when we went over the [OpenGL Pipeline](../undesignated_pages/opengl_pipeline.md). What came right before *Rasterization*?

**HIDE ANSWER** That's right! After all the shaders are processed (excluding the Fragment Shader), the pipeline moves into *Primitive Assembly*. But what is a *primitive*?

*Primitives* are "shapes" that tell OpenGL how to group/treat vertices passed into the pipeline after a *draw* call (e.g. `glDrawArrays()`). We have already seen one such *primitive*: `GL_POINTS`. By specifying `GL_POINTS` we instructed OpenGL to treat each vertex as a single point and render it. You can imagine that this would get very tedious if we could only render single points to draw our scenes.

Fortunately, OpenGL supports *many* different types of primitives.

# Primitive Types

OpenGL has access to *eight* "core" primitive types when using the programmable pipeline.[^1] We won't be using all of them in this course, but it is useful to have some knowledge of each. We can categorize these into four distinct groups:

* Point
* Line
* Triangle
* Patch

Let us take a look at each in turn.

## Point Primitive

### GL_POINTS

As mentioned above, we have already seen and used this type of primitive in our [Get to the Point](../week_2/get_to_the_point.md) exploration. When using `GL_POINTS` with any draw call, each vertex will pass through the pipeline as a single point. So if we pass in four vertices, we will get four distinct points.

![GL_POINTS demonstration showing four independent points](../images/undesignated_images/gl_points.svg)

This may sound fairly mundane, but there are some very good uses for `GL_POINTS`. One common usage is in particle systems.[^2] Since points don't have any geometry, they are exceedingly efficient to generate and display. We can adjust the size of each and even have textures applied so they display as sprites.

## Line Primitives

### GL_LINES
### GL_LINE_STRIP
### GL_LINE LOOP

## Triangle Primitives

### GL_TRIANGLES
### GL_TRIANGLE_STRIP
### GL_TRIANGLE_FAN

## Patch Primitive

### GL_PATCHES

[^1]: There are in fact *twelve* primitives total. The four not listed are used exclusively with Geometry Shaders, which won't be used in this course.
[^2]: https://opengl-notes.readthedocs.io/en/latest/topics/particles-mapping/particles.html