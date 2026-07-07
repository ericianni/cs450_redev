# CLOs
* CLO 1 - Explain the different coordinate systems used in computer graphics.
* CLO 6 - Use OpenGL functions to create and apply single and compound transformations.
* CLO 11 - Explain how the GLM computer graphics math library is used to create and apply transformations.

# Introduction

Ok, I am going to level with you. This exploration is likely going to be *super* "heady" and you will likely want to reread it multiple times. Better yet, keep it open in a tab throughout this term. If you don't understand the math, that is fine. You are in good company! Just know how to *apply* the math in the context of 3D Graphics and the GLM functions that make it easier to implement.

So, everyone ready? We are about to take a shallow dive into **The Math of 3D Graphics!**

# 3D Graphics Math (it won't be *that* bad)

What you are about to see is basically *Linear Algebra*[^1] If you have taken a course in the subject, then YAY for you! If you haven't taken such a class, or are like me and took it *decades* ago, don't worry, things will be OK!

## Points

This is likely the easiest concept to grasp, so it's a good place to start! A *point* is just that: *a point in space*. You have surely plotted countless points across your academic career. They show up when graphing equations on you calculator. They show up when plotting data on a graph for a presentation.

Most of these points are presented in a *2-D plane*. This means, they have both an *X* and a *Y* coordinate. These two coordinates allow us to describe a point's position on the x and y axis. As we mentioned in our [Coordinate Systems](./coordinate_systems.md) exploration, we need to expand our axis to include a *Z*. 

This means that *any* vertex we want to place in our scene requires an *X*, *Y*, an *Z* coordinate. Given this information, what *type* do you think we should use: `vec3` or `vec4`?

**HIDE ANSWER: If you guessed `vec3` you are making the purely logical choice. Sadly, you are incorrect in the context of 3D Graphics Math. We actually want a `vec4`**

"Hold up!" I can hear you shout. If we need three numbers to represent a 3D point, why *wouldn't* we use a `vec3`? As with many things in 3D Graphics, we make decisions based on ease of use. In this case, we want to use a `vec4` so we can leverage something called *Homogeneous Coordinates*.

This is just a fancy term for adding an element to the coordinate vector to make the math easier. For our purposes, we will be adding `W`, which will typically be set to `1`. We can create a point centered on the origin by calling:

```C++
glm::vec4 point(0.0f, 0.0f, 0.0f, 1.0f);
```

OK, so we now know how to create a `vec4` to hold our *homogenous coordinates*, but what math is made easier because of it? Well, *Matrix Math* of course! (editor's note: great segue!)

## Matrices


## Vectors
## Transforms
## Local and World + Model Matrix
## Eye Space + View Matrix
## Projection Matrices
## Model-View-Projection
# GLM 
## Useful GLM Operations

[^1]: [Linear Algebra](https://www.merriam-webster.com/dictionary/linear%20algebra): a branch of mathematics that is concerned with mathematical structures closed under the operations of addition and scalar multiplication and that includes the theory of systems of linear equations, matrices, determinants, vector spaces, and linear transformations
