package  
{
	import flash.geom.Rectangle;
	import starling.display.Image;
	import starling.display.MovieClip;
	import starling.textures.Texture;
	import starling.textures.TextureAtlas;
	import starling.utils.AssetManager;
	public class AssetLib 
	{		
		//public static const BIG_SHIP_ATLAS:String = "BIG_PIRATES_SHIP";
		
		
		private static var assets: AssetManager; // ассет-менеджер
		
		public static var isAssetsLoaded:Boolean = false;
		
		/**
		 *  Инициализация
		 */
		public static function init(assetManager:AssetManager, onComplete:Function):void
		{
			assets = assetManager;
			
			assets.loadQueue(function(ratio:Number):void
				{
					if (ratio == 1.0)
					{
						isAssetsLoaded = true;
						onComplete();
					}
				});
		}
		

		public static function createObject(key:String, state:String):MovieClip {
			var atlas:TextureAtlas = assets.getTextureAtlas(key); 
			if (atlas){
				var textures:Vector.<Texture> = atlas.getTextures(state);
				if (textures.length > 0) {					
					var movie:MovieClip = new MovieClip(textures, 30);
					movie.x = -movie.width / 2;
					movie.y = -movie.height / 2;					
					return movie;
				}
			}
			return null;
		}		
	}
}