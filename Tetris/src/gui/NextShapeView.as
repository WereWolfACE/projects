package gui 
{
	import flash.display.Sprite;
	import flash.text.TextField;
	/**
	 * ...
	 * @author Marshalova Valeriya 
	 */
	public class NextShapeView extends Sprite
	{		
		private var _shapeText:TextField;
		private var _nextShapeSprite:Sprite;
		
		public function NextShapeView() 
		{			
			_shapeText = View.createLabel("Tahoma", 14, 0x0, false, true, false, false, false, true, "center", "center", 0, 0, Config.getLocalizationText(Config.LOCAL_TEXT_NEXT_SHAPE));
			_shapeText.width = Config.PANEL_WIDTH;
			
			_nextShapeSprite = new Sprite();				
			addChild(_shapeText);
			addChild(_nextShapeSprite);
		}
		
		public function set nextShapeSprite(value:Sprite):void 
		{
			if (contains(_nextShapeSprite)) removeChild(_nextShapeSprite);
			_nextShapeSprite = value;
			_nextShapeSprite.y = _shapeText.y + _shapeText.height + 10;
			_nextShapeSprite.x = (this.width - _nextShapeSprite.width) / 2;
			addChild(_nextShapeSprite);
		}
		
		
		
	}

}