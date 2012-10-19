// TODO: make sure this handles audio as well as images implicitly.

function AssetHandler()
{
	this.assets_list = [];
	this.assets = {};
	this.to_load = [];

	this.loading = false;
	this.total_assets = 0;
	this.loaded_assets = 0;
	this.complete = false;

	this.TagAsset = function(tag, filename)
	{
		this.to_load.push([tag, filename]);
		this.total_assets += 1;
		return this.total_assets;
	}

	this.Load = function()
	{
		if(this.loading == false)
		{
			if(this.total_assets == this.loaded_assets)
			{
				return (1);
			}

			this.loading = true;
			setTimeout(bind(this, this._loader), 100);
		}
		else if(this.loading == true)
		{
			if(this.loaded_assets == this.total_assets)
			{
				this.loading = false;
				this.complete = true;
			}
			return (this.loaded_assets / this.total_assets);
		}
	}

	this._loader = function()
	{
		for(var i = 0; i < this.total_assets; i++)
		{
			var tag = this.to_load[i][0];
			var filename = this.to_load[i][1];

			this.assets_list[i] = new Image();
			this.assets_list[i].src = filename;

			this.assets[tag] = this.assets_list[i];

			this.assets_list[i].onload = bind(this, function(evt)
			{
				this.loaded_assets += 1;
			});

			// TODO: finish makine sure the object works in it's entirety
			this.assets_list[i].onerror = function()
			{
				console.log("ERROR LOADING IMAGES! ", this.assets_list[i]);
				alert("asset failed to load.");
			}

			this.assets_list[i].onabort = function()
			{
				console.log("IMAGE LOADING ABORTED! ", this.assets_list[i]);
			}
		}
	}

}