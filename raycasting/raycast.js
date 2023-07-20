const TILE_SIZE = 32;
const MAP_ROWS = 11;
const MAP_COLS = 15;

const W_WIDTH = TILE_SIZE * MAP_COLS;
const W_HEIGHT = TILE_SIZE * MAP_ROWS;

//player's view angle
const FOV_ANGLE = 60 * (Math.PI / 180);

//tickness of pixels for the wall
const WALL_STRIP = 10;
//has to fill the whole width of the screen
const NUM_RAYS = W_WIDTH / WALL_STRIP;

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
    hasWallAt(x, y) {
        if (x < 0 || x > W_WIDTH || y < 0 || y > W_HEIGHT) {
            return true;
        }
        //converts the x and y to the grid position
        var mapGridIndexX = Math.floor(x / TILE_SIZE);
        var mapGridIndexY = Math.floor(y / TILE_SIZE);
        //checks if the position is a wall returns true if 1 and false if 0
        return this.grid[mapGridIndexY][mapGridIndexX] == 1;
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

class Player {
    constructor () {
        this.x = W_WIDTH / 2;
        this.y = W_HEIGHT / 2;
        this.radius = 3;
        this.turnDirection = 0; // -1 for left, +1 for right
        this.walkDirection = 0; // -1 for back, +1 for front
        this.rotationAngle = Math.PI / 2; //90 degrees
        this.moveSpeed = 2.0; //2 pixels per frame
        this.rotationSpeed = 2 * (Math.PI / 180); //2 degrees per frame _> converted to radians
    }
    update() {
        //update the rotation of the view
        this.rotationAngle += this.turnDirection * this.rotationSpeed;
        //update player position based on turnDirection and walkDirection
        var moveStep = this.walkDirection * this.moveSpeed;
        var posPlayX = this.x + Math.cos(this.rotationAngle) * moveStep;
        var posPlayY = this.y + Math.sin(this.rotationAngle) * moveStep;
        //check if the new position is a valid position - not colliding with a wall
        if (grid.hasWallAt(posPlayX, posPlayY) == false) {
            this.x = posPlayX;
            this.y = posPlayY;
        }
    }
    render() {
        noStroke();
        fill("red");
        circle(this.x, this.y, this.radius);
        stroke("red");
        line(this.x, this.y,
             this.x + Math.cos(this.rotationAngle) * 30, 
             this.y + Math.sin(this.rotationAngle) * 30);
    }
}

class Ray {
    constructor (rayAngle) {
        this.rayAngle = rayAngle;
    }
    render() {
        stroke("rgba(255, 0, 0, 0.3)");
        line(player.x, player.y,
             player.x + Math.cos(this.rayAngle) * 30,
             player.y + Math.sin(this.rayAngle) * 30);
    }
}

var grid = new Map();
var player = new Player();
var rays = [];

function keyPressed() {
    if (keyCode == UP_ARROW) {
        player.walkDirection = +1;
    } else if (keyCode == DOWN_ARROW) {
        player.walkDirection = -1;
    } else if (keyCode == LEFT_ARROW) {
        player.turnDirection = -1;
    } else if (keyCode == RIGHT_ARROW) {
        player.turnDirection = +1;
    }
}

function keyReleased() {
    if (keyCode == UP_ARROW) {
        player.walkDirection = 0;
    } else if (keyCode == DOWN_ARROW) {
        player.walkDirection = 0;
    } else if (keyCode == LEFT_ARROW) {
        player.turnDirection = 0;
    } else if (keyCode == RIGHT_ARROW) {
        player.turnDirection = 0;
    }
}

function castAllRays() {
    var columnId = 0;
    
    // start first ray subtracting half of the FOV
    var rayAngle = player.rotationAngle - (FOV_ANGLE / 2);

    rays = [];

    //loop all columns casting the rays for each
    for (var i = 0; i < NUM_RAYS; i++) {
        var ray = new Ray(rayAngle);
        //ray.cast();
        rays.push(ray);

        rayAngle += FOV_ANGLE / NUM_RAYS;
        columnId++;
    }
}

//TODO: init all objs once
function setup() {
    createCanvas(W_WIDTH, W_HEIGHT);
}

//TODO: update all objs before rendering next frame
function update() {
    player.update();
    castAllRays();
}

//TODO: draw all objs frame by frame
function draw() {
    update();

    grid.render();
    for (ray of rays) {
        ray.render();
    }
    player.render();
}