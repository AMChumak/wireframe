## AMC Wireframe

### Author:

Anton Chumak aka [@AMChumak](https://github.com/AMChumak). Student of FIT NSU, programmer,
3D modeler, cyclist, just a nice guy!

## About program

AMC Wireframe - modern rotation shape editor. It allows you to create 
shaping B-splines and view models in a 3d viewer.

## Structure

The application contains of 2 windows: [3D Viewer](#3d-viewer) and [Forming shape editor](#forming-shape-editor).

### 3D Viewer
There you can see 3D model. Use wheel to zoom in and zoom out. 
Drag points on model with pressed mouse button to rotate it in space.

### Toolbar

The toolbar is used for quick access to the editor's functions.
The functions are divided into 2 main groups:
[File options](#file-options) and [Edit tools](#graphic-tools).

#### File options

- **New file** `Creates new session`
- **Open file** `Opens an existing image file in .s3d format`
- **Save file** `Saves session into file in .s3d format`

#### Edit tools

- **Editor** `Opens forming shape editor`
- **Reset** `Resets rotation`

### Menu

The menu contains all the functionality of the application.
It consists of three sections: [File](#file), [Edit](#edit), and [About](#about).

#### File

It contains functions related to working with files:

- **New file** `Creates new session`
- **Open file** `Opens an existing image file in one of the formats (.png, .jpg, .bmp, .gif)`
- **Save file** `Saves session into file in .png format`

#### Edit

It contains functions related to edit model or its appearance.

- **Editor** `Opens forming shape editor`
- **Reset** `Resets rotation`

#### About

- **About**  `Opens this text :)`


### Forming shape editor

This window allows you to set forming shape.
> [HINT] The 3D model is updated online during changes in the editor

#### Main parameters:
- **N** `Count of points in one segment of B-spline`
- **K** `Count segments of B-spline`
- **M** `Count of forming polylines in 3d model`
- **M1** `Count of points in one arc, that coonects neighbour forming polylines`

#### How to change point position
- Drag points with mouse
- Double-click on point and set coordinates in menu

### How to add points
- Turn on radio button 'add point mode' and when your clicks will create new points
- Increase parameter K

> [HINT] The 'add point mode' is automatically activated when the control button is pressed.

### How to delete points
- Choose point and press 'delete'
- Decrease parameter K

> [HINT] If you delete highlighted point, when highlight will move to next point. 
> You can delete some next points with multi-click 'delete'.

#### How to zoom
- Use wheel to change zoom
- Use buttons in bottom panel 

### How to move camera
- Press mouse in free space and move it!