bullet = {
	Type = "Projectile",
	ComponentGraphics = {
		name = "temp"
	},
	ComponentPosition = {
		position = { -500, -100 },
		size = { 5, 5 }
	},
	ComponentMovement = {
		speed = 1000
	},
	ComponentDamage = {
		damage = 40,
		range = 800
	},
	ComponentCollision = {
		width = 5,
		height = 5,
		collide = function(this, second)
			Damage(this, second)
			Destroy(this)
			Log(this)
		end
	}
}