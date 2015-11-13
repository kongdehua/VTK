# Computer Graphics Primer

Computer graphics is the foundation of data visualization. Practically speaking, we can say that visualization is the process that transforms data into a set of graphics primitives. The method of computer graphics are then used to convert these primitives into pictures or animations. This chapter discusses basic computer graphics principles. We begin by describing how lights and physical objects interact to form what we see. Then we examine how to simulate these interactions using computer graphics techniques. Hardware issues play an important role here since modern computers have built-in hardware support for graphics. Then chapter concludes with a series of examples that illustrate our object-oriented model for 3D computer graphics.

## 1. Introduction

Computer graphics is the process of generating images using computers. We call this *process rendering*. There are many types of rendering processes, ranging from 2D paint programs to sophisticated 3D techniques. In this chapter we focus on basic 3D techniques for visualization.

We can view rendering as the process of converting graphical data into an image. In data visualization our goal is to transform data into graphical data, or graphics primitives, that are then renderer. The goal of our rendering is not so much image realism as it is information content. We also strive for interactive graphical displays so we can interact with this data. This chapter explains the process of rendering an image from graphical data. We begin by looking at the way lights, cameras, and objects interact in the world around us. From this foundation we explain how to simulate this process on a computer.

### A Physical Description of Rendering
Figure presents a simplified view of what happens when we look at an object, in this
