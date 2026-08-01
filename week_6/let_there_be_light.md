# ClOs
* CLO7 - Use OpenGL to perform light-source shading

# Introduction

You may be surprised to learn that we have covered almost all the *Course Learning Objectives* already. One of the outstanding objectives is to learn about *light-source shading*. Lighting is something that can really make a scene pop, and honestly is not that hard to implement. We are going to start with some theory (and a little math). Then, we will jump into actually putting lights into our scene. You will notice that I use "lighting" and "shading" interchangeably. This is because both terms are used to describe the algorithms we will be using.

# Let There Be Light!

In the real world, light is made up of *photons*. They possess the ability to act as both *waves* and *particles*. As photons bounce around the world, they pick up color, which we can detect once they reach our eyes.[^1]

Modeling this behavior in a digital environment has been one of the most researched aspects of Computer Graphics. If you recall our discussion about [Graphics History](../week_1/graphics_history.md), many of the breakthroughs coming out of Utah dealt with shading/lighting. Until fairly recently, most lighting techniques required varying levels of fakery.[^2]

Part of creating a convincing fake is to examine how light behaves under different conditions. Let's take a moment to categorize a few different "types" of lights.

# Types of Light

## Global Ambient

*Ambient* light simulates light that has bounced around a space so many times that there is no identifiable source. In a way it feels like *ambient* light comes from everywhere and nowhere. Almost every scene will have some level of *ambient* light. No matter how dark the scene is, the user needs to be able to see *something*, so we will need at least some *ambient* light value.

## Directional or Distant

Whereas *ambient* light comes from everywhere, *directional* light has a definite source. This source *outside* our scene: a great distance outside it. The best example of a *directional* light source is the Sun. Rays from the Sun come in at a specific angle, which is consistent across the entirety of your observable area. No matter how far you move in a scene you cannot change the angle of light impacting the objects. *Directional* lights also have the same intensity across the entire scene.

## Positional

*Positional* lights also have a source, but unlike *directional* lights, the source is located *within* our scene. A simple example would be a lightbulb hanging from the ceiling. The angle of light impacting objects will depend on their relative positions in the world. Also, *positional* lights' intensity (brightness) grows smaller the further away from the light position one gets.

## Spotlight

These lights have both a position and a direction. *Spotlights* have a *cone* of light that they emit, and only objects that fall within it are illuminated. I always think of the Bat Signal when I imagine a *spotlight*. The light shoots out in a very tight cone and draws a circle on a cloud to summon The Dark Knight. If you have ever attempted to make shadow puppets, you know that not all light sources are effective; a flashlight works great, but a chandelier does not.

# Modeling Light in OpenGL

In this lesson, we are going to go over three fairly simple (but fairly convincing) lighting models:

* *Flat*
* *Gouraud*
* *Blinn-Phong*

All three of these fall under the *ADS Model* of lighting. The model's name comes from its use of values for:

* *Ambient* - simulates the low-level "everywhere" light value
* *Diffuse* - simulates the brightness based on the angle of light hitting the object
* *Specular* - simulates the shininess of an object by producing highlights

There is a *fourth* value we will be using, but it is attached to the object (not the light source): *shininess*. This value specifies how much light an object should reflect.

These concepts are easier to understand with a visual:

![Screen with an orange teapot with realistic lighting. Arrows point to "ambient", "diffuse", and "specular" areas](../images/week_6/ads_example.png)

Here is a quick rundown of how changing each of these values will affect the look of our objects.

| Ambient ||
| --- | -- |
| Increase | entire object becomes brighter, including the dark side | 
| Decrease | the darkside of the object gets darker |
| Set to Zero | anything part of the object not hit by light is pure black |

| Diffuse ||
| --- | --- |
| Increase | the lit side becomes brighter and you can see more of the shape |
| Decrease | the lit side becomes darker and the object looks flatter |
| Set to Zero | Only ambient and specular light object: mostly dark with highlights |

| Specular | |
| --- | --- |
| Increase | highlight becomes brighter |
| Decrease | highlight becomes dimmer |
| Set to Zero | no shininess, matte finish |

| Shininess ||
|---|---|
| Low (8-16) | Soft highlight |
| Medium (32-36) | "Classic" highlight |
| High (128+) | Very tight and sharp highlight |

# Lighting Math

I know the word "math" can be scary, but we really aren't dealing with a lot of complexity in our algorithms. In the end, we need to identify how much light is hitting each pixel. We will use the following equation:

$$I_{result} = I_{ambient} + I_{diffuse} + I_{specular}$$

$I$ stands for *Intensity* of the light hitting the pixel. This value will then be multiplied by the color value of the pixel to either brighten it or dim it.

$I_{ambient}$ doesn't need to be calculated because it is set directly by us: $Light_{ambient}$. Think of this as the "minimum" intensity value.

$I_{diffuse}$ is calculated using $Light_{diffuse} * cos(\theta)$. $\theta$ is the angle between the light source and the surface normal. We don't actually need to calculate $\theta$ as we can just use $( \widehat{N}  \bullet \widehat{L})$.

<div align="center" markdown="1">

![Diffuse lighting diagram. Theta is between the surface normal and the light source vector](../images/week_6/diffuse.svg)

</div>

Our shaders will be able to calculate $\widehat{N}$ by multiplying the surface normal by the *Normal Matrix* (discussed later). The shaders can also calculate the $\widehat{L}$ by subtracting the *fragment position* from the *light position*. Finally, we only care about surfaces facing the light, so we limit our results to where $cos(\theta) \ge 0$ with $max((\widehat{N}\bullet\widehat{L}))$. Our final equation is:

* $I_{diffuse} = Light_{diffuse} * max(\widehat{N}\bullet\widehat{L})$

$I_{specular}$ is the most complicated to calculate. If you look at the diagram below, you will see that we are looking to determine how much of the reflection ($\overrightarrow{R}$) makes it into the eye ($\overrightarrow{L}$) using $cos^n(\phi)$. 

<div align="center" markdown="1">

![Diffuse lighting diagram. Theta is between the surface normal and the light source vector and between the surface normal and reflection angle. Phi is between the reflection angle and the view vector (vector pointing to the camera)](../images/week_6/specular.svg)

</div>

In this calculation, $n$ is the *shininess* of the object. The higher the shininess, the faster the specular *falloff* (resulting in narrower highlights). As with *Diffusion*, we can skip calculating $\phi$ and jump straight to $max(0,(\widehat{R}\bullet\widehat{V})^n)$. Our final equation is:

* $I_{specular} = Light_{specular} * max(0,(\widehat{R}\bullet\widehat{V})^n)$

Calculating $\overrightarrow{R}$ is fairly simple by *reflecting* $\overrightarrow{L}$ over $\overrightarrow{N}$. It is even simpler to produce $\overrightarrow{V}$ because it is the negative of our *fragment position*, which our shader will have.

**WARNING** When it comes to actually write our shaders, we are going to be *faking* our specular calculations. While it is conceptionally simple to determine $\overrightarrow{R}$, it does require some overhead to produce. IN 1977, James Blinn realized that we only needed $\overrightarrow{R}$ in order to calculate $\phi$.

His new approach generates a new $\overrightarrow{H}$, which is halfway between $\overrightarrow{L}$ and $\overrightarrow{V}$. He realized that the angle $\alpha$ between $\overrightarrow{H}$ and $\overrightarrow{N}$ is roughly $\frac{1}{2}\phi$. Calculating $\overrightarrow{H}$ is simply $\overrightarrow{L} + \overrightarrow{V}$, and $cos(\alpha)$ is calculated with $\widehat{H}\bullet\widehat{N}$. Using the *Blinn-Phong* approach, we can rewrite our equation:

* $I_{specular} = Light_{specular} * max(0,(\widehat{H}\bullet\widehat{N})^n)$

As with all the math in this course, I just want you to be familiar with how it is derived. You won't be asked to reproduce the calculations, you only need to be able to apply the equations. 


[^1]: Technically, they don't "pick up" color. Instead, the energy they have is changed by the materials they interact with. High-energy photons are blue to violet. Low-energy photons are red and orange.
[^2]: *Ray Tracing*, which mimics photons to some degree, has become the gold standard of lighting, with Nvidia and AMD both creating specialized chips to manage the massive calculations. These *Physically Based Rendering* (RBR) techniques are beyond the scope of what we can implement in this course, but are worth exploring.
