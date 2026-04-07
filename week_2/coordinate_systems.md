# Introduction

Whenever you step into a new world, it is important to learn the lay of the land. When it comes to Computer Graphics, we want to make sure we can describe where we are within the scene. In this exploration we are going to discuss *Coordinate Systems*. Make sure you understand these concepts well before moving forward, because knowing which way is up will save you a lot of headache later!

# Coordinate Systems

Likely, you all are very familiar with at least one coordinate system: the *Cartesian Coordinate System*. These are your standard x-y coordinates. A point can be described with by (X, Y), where X details where the point is horizontally, and Y details where the point is vertically. You also likely *instinctively* know which way both Y and X increase (to the right for X and up for Y).

So what happens when we move into 3D space and need to describe a point. If you thought "Add a Z coordinate!" you would be 100% correct! (X, Y, Z) describes a point in a 3D Cartesian coordinate system. Now, think to yourself, which way does Z increase?

If you don't feel confident in your answer, fear not! The truth is, there is no *right* answer. When talking about 3D space, we have to specify *which* of two configurations we are using: *right-hand* vs. *left-hand*.

So how can you tell one from the other? Well, you will see a lot of methods online describing how to use your thumb and fingers to highlight which configuration you are looking at, and many of them are confusing. I am going to share with you the one I use, but if you find something else online that makes more sense, use it. The goal is to have a mental model that *works for you*.

Below you will see two pictures of my hands. Please ignore any cuticle issues and the superglue all over my desk and focus on just the finger positions. The thumb will always be pointing in the positive X direction

![Left hand with thumb extended to the right, the index finger extended up, and the middle finger pointing away from the camera. Each finger is part of an axis labeled X, Y, and Z respectively](../images/week_2/left_handed.jpg)

![Right hand with the thum extended to the right, the index finger extened up, and the middle finger pointing toward the camera. Each finger is part of an axis labeled X, Y, and Z respectively](../images/week_2/right_handed.jpg)