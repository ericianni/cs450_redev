# CLOs
* CLO 1 - Explain the different coordinate systems used in computer graphics.
* CLO 6 - Use OpenGL functions to create and apply single and compound transformations.
* CLO 11 - Explain how the GLM computer graphics math library is used to create and apply transformations.

# Introduction

Ok, I am going to level with you. This exploration is likely going to be *super* "heady" and you will likely want to reread it multiple times. Better yet, keep it open in a tab throughout this term. If you don't understand the math, that is fine. You are in good company! Just know how to *apply* the math in the context of 3D Graphics and the GLM functions that make it easier to implement.

So, everyone ready? We are about to take a shallow dive into **The Math of 3D Graphics!**

# 3D Graphics Math (it won't be *that* bad)

What you are about to see is basically *Linear Algebra*[^1] If you have taken a course in the subject, then YAY for you! If you haven't taken such a class, or are like me and took it *decades* ago, don't worry, things will be OK!

# Points

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

# Matrices

Did you all see the seque? Masterful! Anyway, a *matrix* is basically a series of number organized into rows and columns. Behold! *A Matrix!*

$$
\begin{bmatrix}
A_{00} & A_{01} & A_{02} & A_{03} \\
A_{10} & A_{11} & A_{12} & A_{13} \\
A_{20} & A_{21} & A_{22} & A_{23} \\
A_{30} & A_{31} & A_{32} & A_{33} 
\end{bmatrix}
$$

You have likely seen these before, so we won't need a ton of explaining. The only thing you really need to know about this notation, is that *rows* go left-to-right and *columns* go top-to-bottom. The subscript for each "cell" denotes the row first, and then the column.

We are now going to examine a few *special* matrices that we will be using *a lot* in this course. Again, the goal isn't to make you understand the math, or even to remember the *content* of each matrix. GLM will handle all of that. What you need to know is what they are *for*.

## The Identity Matrix

We are going to be uncharitable and give this matrix another name: *The Do-Nothing Matrix*. Did I just hear you gasp: thinking I am being beyond rude? Why would I rename *The Identity Matrix* to *The Do-Nothing Matrix*?

*Clearly*, I am doing it to be funny, but I am also trying to give you an easy way to remember what it is used for by knowing what it *does*. I mean, can you tell me what you think it does just by looking at it?

$$
\begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

If you can tell just by looking at it, bravo! For the rest of us (including myself), *The Do-Nothing Matrix* (aka *The Identity Matrix*) is the matrix that when multiplied by *any* other point or matrix[^2], that point or matrix remains unchanged: hence, *The Do-Nothing Matrix*.

Why would this be helpful to us? That is a great question...for *you* to try to noodle up a theory. Please don't stress about this question, there are no wrong answers; I just want to have you start thinking about why these things exist. By the end of this exploration, you may be able to guess as to the use before I spell it out.

**Hide Answer: We are going to use the *Identity Matrix* as the "base" for all of the *Transforms* we will be doing later in this exploration. Everything from *moving, scaling, and rotating* your scene will all start with *The Identity Matrix***

GLM allows us to easily create the Identity Matrix with:

```C++
glm::mat4(1.0f);
```

I bet some of you are thinking of asking, 

>"That is all very well and good, but why the dickens do we use a 4x4 matrix instead of a 3x3 matrix? You said we needed to make points `vec4`s to work with Matrix Math! An Identity Matrix can be 3x3, which would have played nicely with the logical `vec3` representation of a 3D point."

My response to that would be, "Wow, you feel strongly about this, and rightfully so!" You must trust the process and wait.[^3]

## Operations

A matrix isn't much use unless we can *do* something with it. We are now going to cover how we can manipulate matrices to produce desired results. Please note, when we talk about matrix operations, we will typically place the "result" on the left-hand side of the `=`. This is because the order in which you do matrix math is important. In the context of 3D graphics, we will be going right-to-left when applying operations. Therefore, our result is placed on the left to mirror how we will assign the resultant value to variables in our code.

### Transpose

When you *transpose* a matrix you "flip" the rows and columns. In other words, Row 1 is rotated to become Column 1, and so on. Below you will see the *T* in the corner of the matrix, which signals *transpose*.

$$
\begin{bmatrix}
A_{00} & A_{10} & A_{20} & A_{30} \\
A_{01} & A_{11} & A_{21} & A_{31} \\
A_{02} & A_{12} & A_{22} & A_{32} \\
A_{03} & A_{13} & A_{23} & A_{33}
\end{bmatrix}=\begin{bmatrix}
A_{00} & A_{01} & A_{02} & A_{03} \\
A_{10} & A_{11} & A_{12} & A_{13} \\
A_{20} & A_{21} & A_{22} & A_{23} \\
A_{30} & A_{31} & A_{32} & A_{33}
\end{bmatrix}^T
$$

So, what do we need to transpose a matrix? 

When we move, rotate, or scale our models using a transformation matrix, it works great for vertices (points). However, it can distort our *normal vectors* — the arrows that point perpendicular to a surface and are critical for lighting.
To correctly transform normals, we multiply them by the *inverse transpose* of the transformation matrix. This keeps them pointing in the right direction.

We will learn about the *inverse* of a matrix shortly.

Both GLM and GLSL have built-in functions to transpose a matrix.

```c++
glm::transpose(mat4); // GLM transpose function

transpose(mat4); // GLSL transpose function
```

### Addition

Matrix *addition* is *very* simple, but it requires that the matrices being added are the same "shape" (i.e. same dimensions). We just need to add the elements from one matrix to the coorisponding elements in the other matrix (see below).

$$
\begin{bmatrix}
A+a & B+b & C+c & D+d \\
E+e & F+f & G+g & H+h \\
I+i & J+j & K+k & L+l \\
M+m & N+n & O+o & P+p
\end{bmatrix}=\begin{bmatrix}
A & B & C & D \\
E & F & G & H \\
I & J & K & L \\
M & N & O & P
\end{bmatrix}+\begin{bmatrix}
a & b & c & d \\
e & f & g & h \\
i & j & k & l \\
m & n & o & p
\end{bmatrix}
$$

Adding two matrices together in with GLM and in GLSL is trivial; we use the `+` operator.

```C++
glm::mat4 result = matA + matB; // GLM matrix addition

mat4 result = matA + matB; // GLSL matrix addition
```

If I am being honest with you, we won't be doing much matrix addition, but we will be doing a lot of what comes next!

### Multiplication

Matrix *multiplication* is going to be our mathematical workhorse when it comes to 3D graphics. While matrix *addition* required the matrices to have the same "shape," matrix *multiplication* requires that the number of *columns* in the first matrix equal the number of *rows* in the second matrix. 

**Nota bene:** The number of rows in the resultant matrix will equal the number of rows in the first matrix. The number of columns in the resultant matrix will equal the number of columns in the second matrix.

Let's take a look at two examples. First, we will look at multiplying two 4x4 matrices. 

**Matrix Multiplication**

$$
\begin{bmatrix}
Aa+Be+Ci+Dm & Ab+Bf+Cj+Dn & Ac+Bg+Ck+Do & Ad+Bh+Cl+Dp \\
Ea+Fe+Gi+Hm & Eb+Ff+Gj+Hn & Ec+Fg+Gk+Ho & Ed+Fh+Gl+Hp \\
Ia+Je+Ki+Lm & Ib+Jf+Kj+Ln & Ic+Jg+Kk+Lo & Id+Jh+Kl+Lp \\
Ma+Ne+Oi+Pm & Mb+Nf+Oj+Pn & Mc+Ng+Ok+Po & Md+Nh+Ol+Pp
\end{bmatrix}=\begin{bmatrix}
A & B & C & D \\
E & F & G & H \\
I & J & K & L \\
M & N & O & P
\end{bmatrix}
*
\begin{bmatrix}
a & b & c & d \\
e & f & g & h \\
i & j & k & l \\
m & n & o & p
\end{bmatrix}
$$

* Concatenation
### Inverse

## Vectors
## Transforms
## Local and World + Model Matrix
## Eye Space + View Matrix
## Projection Matrices
## Model-View-Projection
# GLM 
## Useful GLM Operations

[^1]: [Linear Algebra](https://www.merriam-webster.com/dictionary/linear%20algebra): a branch of mathematics that is concerned with mathematical structures closed under the operations of addition and scalar multiplication and that includes the theory of systems of linear equations, matrices, determinants, vector spaces, and linear transformations
[^2]: Technically, it isn't any other point or matrix, but in our context the statement holds. To learn more about the *actual* operation of the Identiy Matrix see [here](https://www.khanacademy.org/math/algebra-home/alg-matrices/alg-properties-of-matrix-multiplication/a/intro-to-identity-matrices)
[^3]: If you are more the "Screw Waiting" type, you can find the answer to why we need 4x4 matrices in the *Transforms* section under *Translation*.
