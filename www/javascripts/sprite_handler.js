function SpriteHandler(args)
{
	args = (args == undefined) ? {} : args;

	this.args = args

	this.image = args.image;
	this.tile_width  = args.tile_width;
	this.tile_height = args.tile_height;

	this.frame_count = this.image.width / this.tile_width;

	this.frame_index = 0;

	this.frame_life = args.frame_life || 10;
	this.current_frame = 0;

	this.draw = function(context, x, y)
	{
		var starting_x = this.frame_index * this.tile_width;
		var starting_y = 0;

		context.drawImage(args.image, starting_x, 0, this.tile_width, this.tile_height, x, y, this.tile_width, this.tile_height);

		this.current_frame++;

		if(this.current_frame == 10)
		{
			this.current_frame = 0;
			this.nextImage();
		}

	}

	this.nextImage = function()
	{
		this.frame_index = (this.frame_index + 1) % this.frame_count;
	}
}
