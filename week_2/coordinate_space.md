# CLOs
* CLO1 - Explain the different coordinate systems used in computer graphics

# Indtroduction

We just covered Left-Handed vs. Right-Handed coordinate systems. This is very important when visualizing how we will move about our 3D scene, but ultimately, since we are only using one (Right-Handed) in this course, it is not something you need to consider that often. On the other hand, what we are about to discuss needs to be at the front of your mind every step of the way.

# Space

Who is excited to talk about *space*!?

<div align="center" markdown="1">

![Image of Galaxy UGC 2885 provided by esahubble.org](../images/week_2/space.jpg)

</div>

No, not that kind of *space*! Hopefully you are all still excited, because we need to talk about *Object, World, View, and Clip Space*.

## Local/Model Space

We have talked a lot about the OpenGL Pipeline and different mathmatical operations we will be using, but we haven't talked much about what we will use these things *for*. At the end of the day, we want to render a 3D scene.

We have discussed that our program will convert the vertices into pixels on a screen. Those vertices will represent all sorts of things. Some of them will be part of the landscape/terrain and fairly static. Others will be part of objects or models that we will animate in order to have them move around the scene.

Typically, each of these elements will be created using 3D modeling software (e.g. Blender or Maya). These models need to have a way to describe where each vertex is in relation to the others, and, typically, we do this by orienting all of them around the origin (0, 0, 0).

This is called *Local Space*: somtimes called *Model Space*.[^7] Each model/object will have its own set of local coordinates. It is even possible to combine multiple objects/models into a larger unit. For example, we may wish to model a head separately from the body and then combine them by using transforms to translate, scale, rotate each element into the correct place. This combined model would also have its own *local space*.