## AMC Paint

### Author: 

Anton Chumak aka [@AMChumak](https://github.com/AMChumak). Student of FIT NSU, programmer, 
3D modeler, cyclist, just a nice guy!  
    

## About program

AMC Paint is a multi-tool graphic editor. You can use it to open and edit images in .png, .jpg, .bmp, and .gif formats.
Saving in .png format is available.

## Structure

The main application window contains a canvas, a toolbar, and a menu.

### Canvas

The canvas is used for displaying and editing images. 
The canvas size can be changed using the `Edit->resize` menu option. 
If the canvas does not fit completely into the screen, 
you can use the scroll bars on the right and bottom of the screen to move around the canvas.

### Toolbar

The toolbar is used for quick access to the editor's functions.
The functions are divided into 3 main groups: 
[File options](#file-options), [Graphic tools](#graphic-tools), and [Pen settings](#pen-settings).

#### File options
- **New file** `Creates new session`
- **Open file** `Opens an existing image file in one of the formats (.png, .jpg, .bmp, .gif)`
- **Save file** `Saves session into file in .png format`
#### Graphic tools
- **Line** `Sets line tool. Choose 2 points on the canvas with 2 clicks.`
- **Fill** `Fills chosen area with chosen color. Use 1 click to set seed pixel of filling`
- **Clean** `Fills canvas with white color`
- **Figures** `Draws choosed figure on the canvas. 
Choose firstly center point and vertex point after that by mouse clicking`

#### Pen settings
- **Width** `Opens menu with pen's with setting`
- **Palette** `Opens dialog with color palette`
- **Colors** `Sets choosed color as current pen's color`


- **About**  `Opens this text :)`

### Menu

The menu contains all the functionality of the application. 
It consists of three sections: [File](#file), [Edit](#edit), and [About](#about).

#### File
It contains functions related to working with files:
- **New file** `Creates new session`
- **Open file** `Opens an existing image file in one of the formats (.png, .jpg, .bmp, .gif)`
- **Save file** `Saves session into file in .png format`

#### Edit
It contains functions related to canvas settings, pen settings and choosing graphic tool.

> [!TIP]
> Current tool is checked in menu/toolbar. 

Canvas settings:
- **Undo** `"Ctrl+Z" undo action`
- **Redo** `"Ctrl+Shift+Z" redo action`
- **Resize** `Resizes canvas`

Pen settings:
- **Width** `Opens menu with pen's with setting`
- **Palette** `Opens dialog with color palette`

Tool choosing:
- **Line** `Sets line tool. Choose 2 points on the canvas with 2 clicks.`
- **Fill** `Fills chosen area with chosen color. Use 1 click to set seed pixel of filling`
- **Insert** `Opens menu with settings of figure, that will be pasted on canvas`

#### About
- **About**  `Opens this text :)`