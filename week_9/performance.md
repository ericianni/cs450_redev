# CLOs

* CLO2 - Describe the names and functions of the elements of the graphics pipeline, as well as the input and output of each stage
* CLO9 - Explain the ways Vertex Buffer Objects can improve your display performance
* CLO10 - Create and use an OpenGL vertex and fragment shader

# Introduction

We have been refactoring our code more and more as the weeks go by. Some of you may find this annoying, but there is a purpose. With the *programmable pipeline* there was a great deal of details to cover before you could get *anything* on the screen. This complexity is great because it gives the developer a great deal of control, but the learning curve is steep.

In an attempt to flatten that curve, I made some design decisions to make conceptualizing the OpenGL process more approachable. Now that we are more familiar with how everything fits together, it is time to start moving towards "best practices." One area our code could use improvement is in how we pass data to the pipeline. We are going to look at *Uniform Buffer Objects* to help make our programs more effecient.

## Uniform Buffer Objects

As the name may suggest, *Uniform Buffer Objects* (*UBO*s) contain *uniforms*. The thing that makes them different, is that *UBO*s can be shared across multiple shader programs. Up until this point, every *uniform* we have set is only accessible in the shader program for which we set it.

So, how does having an *UBO* make this more efficient? You know, that is a great question for you all to ponder!

**HIDE ANSWER: Some data remains constant across many different shaders. It makes little sense to upload it multiple times. Additionally, each time we set a uniform in our application adds overhead to the process. So if we can set it once per program, or once per frame, then we cut down on a lot of wasted CPU processes and GPU memory allocation.**

To drive the point home, go count how many times we query for *Uniform IDs* and set them for our Solar System example last module. That is just a small scene. Now imagine a scene with *thousands* of objects and multiple lights. So we know *how* *UBO*s can improve performance by reducing redundancies of uniforms, but not *which* uniforms are well suited for an *UBO*. Any guesses?

**HIDE ANSWER: We are going to want to convert any uniforms that are updated once per program or once per frame. This includes the *Project Matrix*, *View Matrix*, and lighting data.**

Reducing redundancy is only one of the reasons we want to move these things into *UBO*s.  From a design perspective, it makes little sense for our objects to have any knowledge (even as a pass-through) of these details. It is the shader programs that need these, so it is better to have it handled outside the object class.

## Shader Refactor

Since we are changing how we assign our uniforms, we are going to need to update our shader code. When converting from a standard uniform to a *UBO*, we need to use a different syntax in the shaders themselves. Just like we did with our *vertex attributes*, we are going to use the `layout` keyword to declare our *UBO*s. The difference is that instead of specifying a `location`, we are going to use `std140`.

### std140 Explained

What is `std140`? It is a standardized memory layout. The layout assigns *base alignments* to each data type:

|Type|Size (bytes)|Base Alignment (bytes)|
|--|--|--|
|`float`, `int`, `bool`| 4 | 4 |
|`vec2` | 8 | 8 |
|`vec3` | 12 | 16 |
| `vec4` | 16 | 16 |
| `mat3` | 48[^2] | 16 |
|`mat4` | 64 | 16 |

The *base alignment* alignments are used to calculate (once) the memory offsets for each member of our *Block* (aka the variables inside the layout). It uses these offsets to determine where to look for the next piece of data. If the size of the data doesn't match the *base alignment* (or multiple of) things can go wrong. In these cases we have to "pad" our data. This will be best explained using an example.

On our application side of things, we have a `struct` for our lighting values:

```C++
struct Light {
    glm::vec3 position {0.0f, 0.0f, 0.0f};
    glm::vec3 ambient  {0.15f, 0.15f, 0.15f};
    glm::vec3 diffuse  {0.8f, 0.8f, 0.8f};
    glm::vec3 specular {1.0f, 1.0f, 1.0f};
	glm::vec3 attenuation{ 0.5f, 0.03f, 0.003f };
};
```



```GLSL
layout(std140) uniform VariableName {
    
};
```


### Shader Changes



Let's take a look at our Vertex Shader code and try to figure out which uniforms to convert into *UBO*s.

```GLSL
// Vertex Shader
#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uMV;
uniform mat4 uP;
uniform mat3 uN;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main()
{
    fragPos  = vec3(uMV * vec4(aPos, 1.0));
    normal   = uN * aNormal;
    texCoord = aTexCoord;

    gl_Position = uP * uMV * vec4(aPos, 1.0);
}
```

As we mentioned above, we want to store our *Projection Matrix* and *View Matrix* in a *UBO*. We *could* put each in its own *UBO*, but since they are both related to the *camera*, why not combine them into a single uniform? We can do that easily with:

```GLSL
layout(std140) uniform Camera {
    mat4 uV;
    mat4 uP;
};
```

Reminder: Previously, we sent in our *Model View Matrix* as `uMV`. Now, we are sending the *View Matrix* separate from the *Model Matrix*, so we will have to rename our `uniform mat4 uMV` to `uniform mat uM`. This also means we will need to update what we fill it with in our application. 

Additionally, since the goal is to stop sending the *View* and *Projection* matrices to `draw()`, we can no longer calculate the *Inverse Normal Matrix* inside our object.[^1] This means, we need to remove the `uniform mat3 uN` and calculate it inside `main()`. **NOTA BENE**: we want to keep our `aNormal` unchanged; these are *vertex attributes* and need to be updated every `draw()` call.

After making all the changes, we will have the following Vertex Shader code:

```GLSL
// Vertex Shader
#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout(std140) uniform Camera { // New UBO code
    mat4 uV;                    // Separate View Matrix
    mat4 uP;                    // Separate Projection Matrix
};
uniform mat4 uM;                // Separate Model Matrix

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main()
{
    mat4 mv = uV * uM;                      // Create a new Model-View Matrix
    fragPos  = vec3(mv * vec4(aPos, 1.0));  // updated to use the new mv variable
    mat3 n = mat3(transpose(inverse(mv)));  // Calc inv transpose in shader now
    normal = n * aNormal;                   // use the new n variable
    texCoord = aTexCoord;

    gl_Position = uP * mv * vec4(aPos, 1.0); // Updated to use the new mv variable
}
```

Now we need to update our Fragment Shader. Now, this is going to get *weird*, and I am sorry. Therefore, take a close look at the following code before moving on to how we are going to update it.

```GLSL
// Fragement Shader
#version 410 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

struct Light {
    vec3 position;        // already in view space
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};
uniform Light uLight;

float constant  = uLight.attenuation.x;
float linear    = uLight.attenuation.y;
float quadratic = uLight.attenuation.z;

uniform float uTime;

uniform vec3  uObjectColor;
uniform float uShininess;
uniform sampler2D uTex;
uniform int useTexture;

out vec4 FragColor;

void main()
{
    // shader code
}
```

Remember how I said that anything with `std140` has to be 16-bytes (or multiples) or padded to fill the same space in memory? Well, in order to pass in our lighting data, we need to do just that. The first thing we will do is get rid of our `struct` declaration (we will still use it in our application). We are going to replace it, and the uniform `uLight` with the following:

```GLSL
layout(std140) uniform LightBlock {
    vec3  position;
    float pad0;
    vec3  ambient;
    float pad1;
    vec3  diffuse;
    float pad2;
    vec3  specular;
    float pad3;
    vec3  attenuation;
    float pad4;
};
```

You might be asking, "What the heck is going on here?!" This is a fair question, and not one entirely easily to deduce just by looking at it. That said, I am going to ask you to ponder *why* you think we are declaring these `float`s between our `vec3`s.

**HIDE ANSWER: A `vec3` is only 12-bytes, but the GPU will look for a `vec3` starting at offsets that are multiples of 16-bytes. This means that if we don't "pad" our `vec3` with a `float` (4-bytes), then the next `vec3` we try to access

```GLSL
// Fragement Shader
#version 410 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

struct Light {
    vec3 position;        // already in view space
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};
uniform Light uLight;

float constant  = uLight.attenuation.x;
float linear    = uLight.attenuation.y;
float quadratic = uLight.attenuation.z;

uniform float uTime;

uniform vec3  uObjectColor;
uniform float uShininess;
uniform sampler2D uTex;
uniform int useTexture;

out vec4 FragColor;

void main()
{
    // shader code
}
```


[^1]: From our previous version of `draw()`: `glm::mat3 n = glm::mat3(glm::transpose(glm::inverse(mv)));`
[^2]: A `mat3` is stored as three `vec3`s, but each has a *base alignment* of 16: 3 * 16 = 48 (not the expected 36).