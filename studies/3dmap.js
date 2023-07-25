//calculating the value of the projected wall height

var distanceProjPlane = (W_WIDHT / 2) / Math.tan(FOV_ANGLE / 2);
var wallStripHeight = (TILE_SIZE / rays.distance) * distanceProjPlane; 

//correction of "fish eye" -> impression caused by the distortion of the distance
//basically get the player rotation angle and subtract it from the ray angle
var correctDistance = ray.distance * Math.cos(ray.rayAngle - player.rotationAngle);