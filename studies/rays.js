FOV_ANGLE = 60 * (Math.PI / 180);
NUM_RAYS = 320;

function castAllRays() {
	var columnId = 0;

	// start first ray subtracting half of the FOV
	var rayAngle = player.rotationAngle - (FOV_ANGLE / 2);

	// loop all columns casting the rays
	for (var i = 0; i < NUM_RAYS; i++) {
		//call funcrion to cast a single ray
		var ray = new Ray(rayAngle);
		ray.cast(columnId);

		//next ray increases by (FOV_ANGLE / NUM_RAYS)
		rayAngle += FOV_ANGLE / NUM_RAYS;

		columnId++;
	}

}