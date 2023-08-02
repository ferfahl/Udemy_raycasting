const TILE_SIZE = 64;
const MAP_ROWS = 11;
const MAP_COLS = 15;

const W_WIDTH = TILE_SIZE * MAP_COLS;
const W_HEIGHT = TILE_SIZE * MAP_ROWS;

//making the 2D map smallet in the screen
const MINIMAP_SCALE_FACTOR = 0.2;

//player's view angle
const FOV_ANGLE = 60 * (Math.PI / 180);

//tickness of pixels for the wall (if higher, you get less rays, making it easier to render, but it pixelates the walls)
const WALL_STRIP = 1;
//has to fill the whole width of the screen
const NUM_RAYS = W_WIDTH / WALL_STRIP;

class Map {
    constructor() {
        this.grid = [
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1],
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
                rect((MINIMAP_SCALE_FACTOR * tileX),
                     (MINIMAP_SCALE_FACTOR * tileY),
                     (MINIMAP_SCALE_FACTOR * TILE_SIZE),
                     (MINIMAP_SCALE_FACTOR * TILE_SIZE));
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
        fill("blue");
        circle(MINIMAP_SCALE_FACTOR * this.x,
             MINIMAP_SCALE_FACTOR * this.y,
             MINIMAP_SCALE_FACTOR * this.radius);
        // stroke("blue");
        // line(MINIMAP_SCALE_FACTOR * this.x, MINIMAP_SCALE_FACTOR * this.y,
        //      MINIMAP_SCALE_FACTOR * (this.x + Math.cos(this.rotationAngle) * 30), 
        //      MINIMAP_SCALE_FACTOR * (this.y + Math.sin(this.rotationAngle) * 30));
    }
}

class Ray {
    constructor(rayAngle) {
        this.rayAngle = normalizeAngle(rayAngle);
        this.wallHitX = 0; //ray collision with the wall in horizontal
        this.wallHitY = 0; //ray collision with the wall in vertical
        this.distance = 0; //between the player and the wall (lower between the rays)
        this.wasHitVertical = false;

        this.isRayFacingDown = this.rayAngle > 0 && this.rayAngle < Math.PI;
        this.isRayFacingUp = !this.isRayFacingDown;
        this.isRayFacingRight = this.rayAngle < 0.5 * Math.PI || this.rayAngle > 1.5 * Math.PI;
        this.isRayFacingLeft = !this.isRayFacingRight;
    }
    cast() {
        var xintercept, yintercept; //point A in the grid after the ray
        var xstep, ystep;
        ///////////////////////////////
        /////////  HORIZONTAL
        //////////////////////////////
        //todo: perform all the horizontal check

        var foundHorzWallHit = false;
        var HorzWallHitX = 0;
        var HorzWallHitY = 0;

        //find the closest horizontal grid intersection (x, y)
        yintercept = Math.floor(player.y / TILE_SIZE) * TILE_SIZE;
        yintercept += this.isRayFacingDown ? TILE_SIZE : 0;
        xintercept = player.x + (yintercept - player.y) / Math.tan(this.rayAngle);

        //calculate the increment xstep and ystep (delta)
        ystep = TILE_SIZE;
        ystep *= this.isRayFacingUp ? -1 : 1;

        xstep = TILE_SIZE / Math.tan(this.rayAngle);
        xstep *= (this.isRayFacingLeft && xstep > 0) ? -1 : 1;
        xstep *= (this.isRayFacingRight && xstep < 0) ? -1 : 1;

        var nextHorzTouchX = xintercept;
        var nextHorzTouchY = yintercept;

        var control = 0;

        if (this.isRayFacingUp) {
            control = 1;
        }

        //increment xstep and ystep until we find a wall
        while (nextHorzTouchX >= 0 && nextHorzTouchX <= W_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= W_HEIGHT) {
            if (grid.hasWallAt(nextHorzTouchX, nextHorzTouchY - control)) {
                //found a wall
                foundHorzWallHit = true;
                HorzWallHitX = nextHorzTouchX;
                HorzWallHitY = nextHorzTouchY;
                break;
            } else {
                nextHorzTouchX += xstep;
                nextHorzTouchY += ystep;
            }
        }
        ///////////////////////////////
        /////////  VERTICAL
        //////////////////////////////
        //todo: perform all the vertical check

        var foundVertWallHit = false;
        var VertWallHitX = 0;
        var VertWallHitY = 0;

        //find the closest vertical grid intersection (x, y)
        xintercept = Math.floor(player.x / TILE_SIZE) * TILE_SIZE;
        xintercept += this.isRayFacingRight ? TILE_SIZE : 0;
        yintercept = player.y + (xintercept - player.x) * Math.tan(this.rayAngle);

        //calculate the increment xstep and ystep (delta)
        xstep = TILE_SIZE;
        xstep *= this.isRayFacingLeft ? -1 : 1;

        ystep = TILE_SIZE * Math.tan(this.rayAngle);
        ystep *= (this.isRayFacingUp && ystep > 0) ? -1 : 1;
        ystep *= (this.isRayFacingDown && ystep < 0) ? -1 : 1;

        var nextVertTouchX = xintercept;
        var nextVertTouchY = yintercept;

        var control = 0;

        if (this.isRayFacingLeft) {
            control = 1;
        }

        //increment xstep and ystep until we find a wall
        while (nextVertTouchX >= 0 && nextVertTouchX <= W_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= W_HEIGHT) {
            if (grid.hasWallAt(nextVertTouchX - control, nextVertTouchY)) {
                //found a wall
                foundVertWallHit = true;
                VertWallHitX = nextVertTouchX;
                VertWallHitY = nextVertTouchY;
                break;
            } else {
                nextVertTouchX += xstep;
                nextVertTouchY += ystep;
            }
        }
        ///////////////////////////////
        /////////  DEFINITION
        //////////////////////////////

        //calculate both horizontal and vertical distances
        var HorzHitDistance = (foundHorzWallHit) ? distanceBetweenPoints(player.x, player.y, HorzWallHitX, HorzWallHitY) : Number.MAX_VALUE;
        var VertHitDistance = (foundVertWallHit) ? distanceBetweenPoints(player.x, player.y, VertWallHitX, VertWallHitY) : Number.MAX_VALUE;

        //choose the smallest one, only store it
        if (VertHitDistance < HorzHitDistance) {
            this.wallHitX = VertWallHitX;
            this.wallHitY = VertWallHitY;
            this.distance = VertHitDistance;
            this.wasHitVertical = true;
        } else {
            this.wallHitX = HorzWallHitX;
            this.wallHitY = HorzWallHitY;
            this.distance = HorzHitDistance;
        }
    }
    render() {
        stroke("rgba(255, 0, 0, 0.3)");
        line(MINIMAP_SCALE_FACTOR * player.x, MINIMAP_SCALE_FACTOR * player.y,
             MINIMAP_SCALE_FACTOR * this.wallHitX,
             MINIMAP_SCALE_FACTOR * this.wallHitY);
    }
}

var grid = new Map();
var player = new Player();
var rays = [];

function render3DProjectedWalls() {
    //loop for rays[]
    for (i = 0; i < NUM_RAYS; i++) {
        var ray = rays[i]; // current ray

        //correct wall distance
        var rayDistance = (ray.distance * Math.cos(ray.rayAngle - player.rotationAngle));

        var distanceProjectionPlace = (W_WIDTH / 2) * Math.tan(FOV_ANGLE / 2); // distance from player to the projection plane

        var wallStripHeight = (TILE_SIZE / rayDistance) * distanceProjectionPlace; //projected wall size

        //transparency of walls, from closer and farther
        // var alpha = 180 / rayDistance; 
        var alpha = 1.0;

        //add colors instead of transparency
        var tone = ray.wasHitVertical ? 255 : 180;

        //draw the rectangle on screen
        fill("rgba(" + tone + ", " + tone + ", " + tone + ", " + alpha + ")");
        noStroke();
        rect(i * WALL_STRIP, 
            (W_HEIGHT / 2) - (wallStripHeight / 2),
            WALL_STRIP, wallStripHeight)
    }

}

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
    // start first ray subtracting half of the FOV
    var rayAngle = player.rotationAngle - (FOV_ANGLE / 2);

    rays = [];

    //loop all columns casting the rays for each
    for (var col = 0; col < NUM_RAYS; col++) {
        var ray = new Ray(rayAngle);
        ray.cast();
        rays.push(ray);

        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}

//sanitizing the value of the angle
function normalizeAngle(angle) {
    angle = angle % (2 * Math.PI);
    if (angle < 0) {
        angle = (2 * Math.PI) + angle;
    }
    return angle;
}

//calculate distance between points
function distanceBetweenPoints(x1, y1, x2, y2) {
    return Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
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
    clear();
    update();

    render3DProjectedWalls();

    grid.render();
    for (ray of rays) {
        ray.render();
    }
    player.render();
}