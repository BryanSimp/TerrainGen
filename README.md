# 3D Terrain Generator
For this project, I used a perlin noise function to determine the height of y using the x and z coordinates.

I started out by creating a grid using a width and height of 100 and then a grid resolution of 100 so that every square
has a length and width of 1 and the center of the grid is at x:0 and z:0.

Then I adjusted the color of the Terarin based on the Y value to make it look like the High points are lighter and the
low points are darker to give it some depth.

The camera is controlled using W A S D to move forward left back and right. And to look around you use the left 
and right arrow keys.To move up and down you use the up and down arrow keys.

To run open up my project into visual studio 2022 and make sure you have the directx sdk downloaded on your system.

EDIT: In modelclass.cpp in the InitializeBuffers function f you adjust the terrainWidth and terrainHeight 
values to about half the resolution and then increase the heightScale to 50 and increase the noiseScale to 0.9 
you can get something that looks like a grassy field.

This is my first finished project using directX11
Completion Date: 10/17/2024
