const TILE_SIZE = 32;
const MAP_ROWS = 11;
const MAP_COLS = 15;

const W_WIDTH = TILE_SIZE * MAP_COLS;
const W_HEIGHT = TILE_SIZE * MAP_ROWS;

class Map {
    constructor() {
        this.grid = [
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        ];
    }
    render() {
        //increases the line
        for (var i = 0 ; i < MAP_ROWS; i++) { 
            //increases the column
            for (var j = 0 ; j < MAP_COLS; j++) { 
                //sets tileX as the column number * the tile size
                var tileX = j * TILE_SIZE;
                //sets tileY as the line number * the tile size
                var tileY = i * TILE_SIZE;
                //sets 1 as black and 0 as white
                var tileColor = this.grid[i][j] == 1 ? "#222" : "#fff";
                //sets a stroke color
                stroke("#222");
                //uses the tileColor as setting for filling the image
                fill(tileColor);
                //creates a rectangle with the infos setted above
                rect(tileX, tileY, TILE_SIZE, TILE_SIZE);
            }
        }
    }
}

var grid = new Map();

//TODO: init all objs once
function setup() {
    createCanvas(W_WIDTH, W_HEIGHT);
}

//TODO: update all objs before rendering next frame
function update() {
}

//TODO: draw all objs frame by frame
function draw() {
    update();

    grid.render();
}