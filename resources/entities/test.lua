test = {
	Type = "TestFile",
	ComponentCollision = {
		width = 20,
		height = 20,
		collide = function(this, second)

		end
	},
	ComponentGraphics = {
		name = "someGraphicsFile.png"
	},
	ComponentPosition = {
		position = { 500, 100 },
		size = { 20, 20 }
	},
	ComponentMovement = {
		speed = 300
	},
	ComponentSelect = {
		selectable = true,
		selectBox = { 20, 20 }
	},
	ComponentWeapon = {
		type = "bullet",
		rateOfFire = 1
	},
	ComponentHealth = {
		health = 100
	}
}