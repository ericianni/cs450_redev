# CLOs
* CLO8 - Use OpenGL to perform texture mapping.

# Introduction

It is time to return to *Textures*. When we created our Object class, we included code to load textures. I had you play around with a texture or two. At the time, we didn't discuss any specifics about how OpenGL handles the application of textures. It is time to fill in those gaps!

# What is a Texture Mapping

Simply put, *Texture Mapping* is the process of painting an image across the surface of a model. Textures are a quick and easy way to add realism to a scene. *Texture Mapping* is so important that GPUs have dedicated hardware units to support it.

Almost any image file can be used as a texture, as long as it is loaded into memory.[^1] While it is possible to write custom code to handle this, we are going to be using *SOIL2* so we can focus on *using* textures right away (more on usage later).

So, *how* do we get OpenGL to apply the textures how we want? We use *Texture Coordinates*. Think back to the code we wrote to load `.obj` files. We created three UBOs to house our *vertex attributes*. The first one is for our vertex *position*. The second one is for our vertex *normals*. The last one is for our *texture coordinates*.

# Texture Coordinates

For our introduction to *Texture Coordinates*, please take a gander at this photo my wife took of me in Colonial Williamsburg.

![Image of Eric wearing a coachman hat in the snow](../images/week_7/dapper_eric.png)

*Any* image can be a texture, including the above dapperness. All you have to do is apply our *texture coordinate* system. The horizontal and vertical axis are labeled $s$ and $t$ respectively.[^2] The range for each axis is [0,1].

Why do you suppose we use [0,1] as the range?

**HIDE ANSWER: [0,1] provides a way to applying *relative* coordinates onto *any* image. If we had to specify based on the number of pixels, we wouldn't be able to use texture files of different sizes or dimensions without changing the object files themselves.**

In the object file, we specify key *texture coordinates* and then assign them to specific vertices. Depending on the way the object file is written, it changes how the texture is applied. Let's take a look at two different cube objects, each with a different way of assigning textures.

![Gif of two cubes with the Eric image wrapped in different ways. Left: entire image is fit onto each face. Right: the image is wrapped around the cube](../images/week_7/dapper.gif)

That is a bit more horrific than I had imagined... Push past it and focus on how each cube wraps the image differently.

* Left Cube: The entire image is *squashed* to make it fit on each face.
* Right Cube: The image is broken up across all six faces. If you look closely, the Left Cube doesn't map the entire image to the cube.

How was this achieved? Let's take a look at the `.obj` file for each. We will examine them in pieces.

## OBJ File Vertices

|| Left Cube| Right Cube|
|----------|----------|-----|
| Vertices | # 8 Vertices<br>v 1.000000 1.000000 -1.000000<br>v 1.000000 -1.000000 -1.000000<br>v 1.000000 1.000000 1.000000<br>v 1.000000 -1.000000 1.000000<br>v -1.000000 1.000000 -1.000000<br>v -1.000000 -1.000000 -1.000000<br>v -1.000000 1.000000 1.000000<br>v -1.000000 -1.000000 1.000000<br> | # 8 Vertices<br>v 1.000000 1.000000 -1.000000<br>v 1.000000 -1.000000 -1.000000<br>v 1.000000 1.000000 1.000000<br>v 1.000000 -1.000000 1.000000<br>v -1.000000 1.000000 -1.000000<br>v -1.000000 -1.000000 -1.000000<br>v -1.000000 1.000000 1.000000<br>v -1.000000 -1.000000 1.000000 |

As you can see, both objects have the same vertices (prefixed with `v`). Why only 8 when we tell our `glDrawArrays()` to draw 36?

**HIDE ANSWER: Remember, we aren't able to reuse vertices with our current approach. Since each side of the cube is made up of two triangles, that means that each corner appears in three different faces: $8x3=36$.**

## OBJ File Texture Coordinates

|| Left Cube| Right Cube|
|----------|----------|-----|
| TexCoords | # 4 Texture Coordinates (Full 0-1 coverage)<br>vt 0.000000 0.000000<br>vt 1.000000 0.000000<br>vt 0.000000 1.000000<br>vt 1.000000 1.000000 |  # 14 Texture Coordinates<br>vt 0.875000 0.500000<br>vt 0.625000 0.750000<br>vt 0.625000 0.500000<br>vt 0.375000 1.000000<br>vt 0.375000 0.750000<br>vt 0.625000 0.000000<br>vt 0.375000 0.250000<br>vt 0.375000 0.000000<br>vt 0.375000 0.500000<br>vt 0.125000 0.750000<br>vt 0.125000 0.500000<br>vt 0.625000 0.250000<br>vt 0.875000 0.750000<br>vt 0.625000 1.000000|

These are our *Texture Coordinates* (prefixed with `vt`). Notice, that the Left Cube only needs four coordinates vs. 14 for the Right Cube. This is because we are only using the corners of the image. The Right Cube uses something called *box projection*. Take a look at how both look inside a UV mapper:

|Left Cube | Right Cube |
|--|--|
|![Image showing full image texture mapping](../images/week_7/dapper_crate.png)|![Image showing box projection texture mapping](../images/week_7/dapper_box_proj.png)|

You can see how each triangle are mapped. In the Left Cube, the texture coordinates are plotted at the corners so only have a value of 0.0 or 1.0. On the other hand, the Right Cube has texture coordinates mapped all over the image, so has values that run the gamut from 0.0 to 1.0.

It is outside the scope of this course, but it is possible to create some very complex mappings.[^3]

![Complex mapping from Blender Manual](../images/week_7/blender_example.png)

## OBJ File Face Normals

|| Left Cube| Right Cube|
|----------|----------|-----|
| TexCoords | # 6 Face Normals<br>vn 0.000000 0.000000 1.000000<br>vn 0.000000 0.000000 -1.000000<br>vn 0.000000 1.000000 0.000000<br>vn 0.000000 -1.000000 0.000000<br>vn 1.000000 0.000000 0.000000<br>vn -1.000000 0.000000 0.000000 | # 6 Face Normals<br>vn 0.000000 0.000000 1.000000<br>vn 0.000000 0.000000 -1.000000<br>vn 0.000000 1.000000 0.000000<br>vn 0.000000 -1.000000 0.000000<br>vn 1.000000 0.000000 0.000000<br>vn -1.000000 0.000000 0.000000|

These normals (prefixed with `vn`) are the same for both models, which makes sense as they are both cubes.

## OBJ File Faces

|| Left Cube| Right Cube|
|----------|----------|-----|
| TexCoords | # 12 Triangulated Faces<br># Front Face<br>f 8/1/1 4/2/1 3/4/1<br>f 8/1/1 3/4/1 7/3/1<br># Right Face<br>f 4/1/5 2/2/5 1/4/5<br>f 4/1/5 1/4/5 3/3/5<br># Back Face<br>f 2/1/2 6/2/2 5/4/2<br>f 2/1/2 5/4/2 1/3/2<br># Left Face<br>f 6/1/6 8/2/6 7/4/6<br>f 6/1/6 7/4/6 5/3/6<br># Top Face<br>f 7/1/3 3/2/3 1/4/3<br>f 7/1/3 1/4/3 5/3/3<br># Bottom Face<br>f 6/1/4 2/2/4 4/4/4<br>f 6/1/4 4/4/4 8/3/4  | # 12 Triangulated Faces<br># Front Face<br>f 3/2/1 8/4/1 4/5/1<br>f 3/2/1 7/14/1 8/4/1<br># Right Face<br>f 1/3/5 4/5/5 2/9/5<br>f 1/3/5 3/2/5 4/5/5<br># Back Face<br>f 5/12/2 2/9/2 6/7/2<br>f 5/12/2 1/3/2 2/9/2<br># Left Face<br>f 7/6/6 6/7/6 8/8/6<br>f 7/6/6 5/12/6 6/7/6<br># Top Face<br>f 5/1/3 3/2/3 1/3/3<br>f 5/1/3 7/13/3 3/2/3<br># Bottom Face<br>f 2/9/4 8/10/4 6/11/4<br>f 2/9/4 4/5/4 8/10/4|

What are we looking at here? As briefly mentioned before, object files only specify each *position*, *texture coordinate*, and *normals* once (for efficiency). In order to define all the actual triangles, the object file enumerates the faces (prefixed with `f`). 

Notice that each face line has three elements? Each of those elements represents a single vertex. Each vertex is defined with three numbers: position/texCoord/normal. Each number references the n<sup>th</sup> element listed in the file for the relevant type. Let's look at an example from Right Cube: `f 3/2/1 8/4/1 4/5/1`.

* Vertex 1
  * position - (1.0, 1.0, 1.0)
  * textCoord - (0.625, 0.75)
  * normal - (0.0, 0.0, 1.0)
* Vertex 2
  * position - (-1.0, -1.0, 1.0)
  * textCoord - (0.375, 1.0)
  * normal - (0.0, 0.0, 1.0)
* Vertex 3
  * position - (1.0, -1.0, 1.0)
  * textCoord - (0.375, 0.75)
  * normal - (0.0, 0.0, 1.0)

These are the values that our object loading code will put into `vertices`, `normals`, and `texCoords` arrays. These will then be passed into the shader using `Vertex Attributes`. This why it is possible to only specify *texture coordinates* at each vertex and still have the entire image show up. More on this in a moment.

As you can see, each of the cubes maps the texture coordinates differently. The Left Cube uses the entire texture image for each side (requiring only four *texCoords*). The Right Cube doesn't reuse any of the texture on any of the faces, so requires many more *texCoords*.

Typically, a 3D artist will create custom UV-mappings (aka ST-mapping) for each *mesh*[^4] they create. This allows for the same texture to be reused multiple times in a scene, but tailored for the specific size and shape of a given object. Then, all of this data is saved together (including the path to the texture).[^5]

# Texture Wrapping

You can also write *texture coordinates* to go beyond the `[0..1]` range. This will result in *tiling/wrapping*. OpenGL supports four types of tiling.

* `GL_REPEAT` - The integer portion of the texture coordinate is ignored and the texture just repeats
* `GL_MIRRORED_REPEAT` - Similar to above, but this time the coordinates are negated when the integer portion is odd
* `GL_CLAMP_TO_EDGE` - Any coordinates less than 0 or greater than 1 are "clamped" to 0 and 1 respectively
* `GL_CLAMP_TO_BORDER` - Any coordinates outside of `[0..1]` are given a specified "border" color

To demonstrate each of these, I am going to modify our cube object's texture coordinates. Below, you will see that I have extended the texture coordinates beyond `[0..1]` to `[-0.5..1.5]`. This will allow us to center my handsome face in the middle of the cube face.

```
# 4 Centered Texture Coordinates (Shifted by -0.5)
vt -0.500000 -0.500000
vt  1.500000 -0.500000
vt -0.500000  1.500000
vt  1.500000  1.500000
```

Can you guess which is which (no cheating!)?


|<!-- -->|<!-- -->|
|--|--|
|![Cube with repeated mirrored tiling](../images/week_7/dapper_mirrored_repeat.png)|![Cube with clamp to edge tiling](../images/week_7/dapper_clamp_edge.png)|
|![Cube with repeated tiling](../images/week_7/dapper_repeat.png)|![Cube with clamp to edge tiling](../images/week_7/dapper_clamp_border.png)|

**HIDE ANSWER: Clockwise from the top-left: `GL_MIRRORED_REPEAT`, `GL_CLAMP_TO_EDGE`, `GL_CLAMP_TO_BORDER`, `GL_REPEAT`.**

You have to specify which tiling method you want to use for both dimensions (s and t). This means, you can mix and match them! What do you think is happening here?

![Cube with tiling repeated along the s dimension and mirrored along the t dimension](../images/week_7/dapper_repeat_and_mirrored_repeat.png)

**HIDE ANSWER: Did you guess `GL_REPEAT` along s and `GL_MIRRORED_REPEAT` along t?**

# Texture Filtering

Remember *interpolation*? As the fragment shader is processing each fragment, it is calculating where to get the correct *texel* (the name for a texture pixel) for each pixel. Since the resolution of our rendered scene likely is different from that of our textures, we have to tell OpenGL how to "guess" the correct *texel* color.

OpenGL has two main Texture Filters:

* `GL_NEAREST` - picks the nearest texel (aka pixel) in the texture and uses that
* `GL_LINEAR` - samples the four closest texel and calculates a weighted average when picking a color

Let's take a look at an exaggerated example of how it works.

||`GL_NEAREST`|`GL_LINEAR`|
|--|--|--|
||![Four colored boxes starting with the top left and going clockwise: white, slate blue, royal blue, pale cyan. A crosshair is on the white box](../images/week_7/filtering_example.png)|![Four colored boxes starting with the top left and going clockwise: white, slate blue, royal blue, pale cyan. A crosshair is on the white box](../images/week_7/filtering_example.png)|
|returns|![A white box](../images/week_7/nearest_color.png)|![A sky blue box](../images/week_7/linear_color.png)


`GL_NEAREST` creates a very *crisp* image with distinct pixelization. Ideal for retro art or UI elements. `GL_LINEAR` creates a smoother blending of the image, making it look more realistic, but blurry when zoomed in close. Below you will find yet more of me, but this time with each of these filters flipped on. See if you can identify which filter was used for which image (no cheating by checking the file name or alt text!).

|Image 1 | Image 2|
|--|--|
|![Close of Eric's eye with LINEAR turned on](../images/week_7/dapper_linear.png)|![Close of Eric's eye with NEAREST turned on](../images/week_7/dapper_nearest.png)|

**HIDE ANSWER: You may have to squint to see that Image 2 has more distinct pixels. This means it uses `GL_NEAREST`.**

# Mipmapping

OpenGL does a *very* good job of interpolating texture coordinates across our objects, but sometimes things get *complicated* and it can start having fits. One thing it can struggle with is drawing textures very far away from the camera or at drastic angles.

The reason for this is that as an object gets smaller on the screen it represents fewer screen pixels. Each pixel now covers a large area of the texture. Very likely multiple texels could map to each pixel and they all likely don't share a color! This issue is called *aliasing* and it can show up as "shimmering" when objects are in motion. Take a look!

![Gif of a brick floor showing shimmering when turning mipmapping off](../images/week_7/no_mipmapping.gif)

If you watch the complete loop (8 seconds) you can briefly see the scene without movement. It doesn't look that bad, but the moment you start moving, the texture immediately starts "shimmering". This is not ideal!

Luckily, OpenGL has a solution: *MipMapping*.[^6] Since one of the causes of this issue is sampling from textures that are too large for the object's on-screen size, a technique was developed that allowed OpenGL to sample from *smaller* versions of the texture under certain conditions. This helps reduce instances where each screen pixel covers too much of a texture.

OpenGL can be instructed to create a *mipmap* of any texture it is asked to load. It then creates a *chain* of successively smaller versions of the same texture (each $\frac{1}{2}$ the size of the previous version). Then when rendering, OpenGL checks to see how fast the *texture coordinates* are changing across the screen.

* Small change - means it is close to the camera (use high resolution image)
* Large change - means it is far away or at steep angle (use lower resolution image)

Here is the same brick floor from before, but this time with *mipmapping* turned on.

![GIF showing the same brick floor with mipmapping on (smoother)](../images/week_7/mipmapping.gif)

Notice that the *aliasing* ("shimmering") is pretty much gone! You may be thinking, "This looks *blurry*" and you wouldn't be wrong. This is because we are using *linear* sampling the same way we could do with *texture filtering* (above). We can actually set multiple types of filtering, but the default is generally what we want to go with:

* `GL_NEAREST_MIPMAP_NEAREST`
* `GL_LINEAR_MIPMAP_NEAREST`
* `GL_NEAREST_MIPMAP_LINEAR`
* `GL_GL_LINEAR_MIPMAP_LINEAR`


[^1]: Technically, it is possible to have a 3D texture, but we are going to focus only on the more common 2D textures.
[^2]: Most modern approaches use $uv$ instead of $st$ for texture coordinates to match the same terminology that 3D artist use in their software. We are sticking with $st$ because that is that is permitted by GLSL: `texture.st` is valid, `texture.uv` is invalid.
[^3]: Notice that the faces of the model are *quads* (not *triangles*). 3D artists prefer to work in quads. Modern graphics enginges (e.g. Unreal and Unity) can convert quads to triangles, which GPUs require. We are not going to be able to do that in this course, so all our faces will be triangles.
[^4]: Mesh just means the vertices that make up an object.
[^5]: You may have noticed the `Failed to load material file(s). Use default material.` warning when running our applications. TinyObjLoader tries to load a *Material* file. These files contain DAS data for lighting, color, shininess, and texture map paths. To keep things approachable, I have not introduced these.
[^6]: It get's it's name from how it stores many versions of a texture in a single texture file: *Multum In Parvo* (Latin for "much in a small place").