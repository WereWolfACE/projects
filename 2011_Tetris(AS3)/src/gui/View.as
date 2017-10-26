package gui
{
	import flash.display.Sprite;
	import flash.filters.GlowFilter;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import logic.TetrisShape;
	
	/**
	 * ...
	 * @author Marshalova Valeriya 
	 */
	public class View
	{			
		public function View() {
			
		}	
		
		public static function createField($fieldData:Array):Sprite {
			var sprite:Sprite = new Sprite();	
			var a:* = Config.SHAPES;
			for (var i:int = 0; i < $fieldData.length; i++) {						
				for (var j:int = 0; j < $fieldData[i].length; j++) {						
					sprite.graphics.beginFill(Config.SHAPES[$fieldData[i][j]].color);					
					sprite.graphics.drawRect(Config.CELL_SIZE * j, Config.CELL_SIZE * i, Config.CELL_SIZE, Config.CELL_SIZE);
					sprite.graphics.endFill();						
				}					
			}			
			return sprite;
		}
		
		/**
		 * Create sprite by count cells in width and height
		 * @param	$width
		 * @param	$height
		 * @return  sprite
		 */
		public static function createBorder($countCellsInWidth:int, $countCellsInHeight:int):Sprite 
		{
			return createWindowBorder(Config.CELL_SIZE * $countCellsInWidth + Config.CELL_SIZE, Config.CELL_SIZE * $countCellsInHeight + Config.CELL_SIZE);
		}
		
		/**
		 * Create sprite by pixels in width and height
		 * @param	$width
		 * @param	$height
		 * @return
		 */
		public static function createWindowBorder($width:int, $height:int):Sprite 
		{
			var sprite:Sprite = new Sprite();					
			
			sprite.graphics.beginFill(Config.BORDER_COLOR);
			sprite.graphics.drawRoundRect(0, 0, $width, $height, 10, 10);
			sprite.graphics.endFill();	
			
			return sprite;
		}
		
		static public function createShape($shape:TetrisShape):Sprite 
		{
			var sprite:Sprite = new Sprite();			
			for each(var p:Point in $shape.arrSquares) 
			{										
				sprite.graphics.beginFill(Config.SHAPES[$shape.type].color);
				sprite.graphics.drawRect(Config.CELL_SIZE * p.y, Config.CELL_SIZE * p.x, Config.CELL_SIZE, Config.CELL_SIZE);
				sprite.graphics.endFill();					
			}			
			sprite.filters = [new GlowFilter(0x0, 1, 2, 2, 3, 2)];
			return sprite;
		}
		
		static public function createDefaultTextField($text:String):TextField
		{
			var textField:TextField = createLabel("Tahoma", 14, 0x0, false, false, false, true, false, false, "right", "right");
			textField.text = $text;
			return textField;
		}
		
		
		public static function createLabel  (
											  $font       : String = null, 
											  $size       : Object = null,
											  $color      : Object = null,
											  $multiline  : Object = null,
											  $wordWrap   : Object = null,
											  $selectable : Object = null,
											  $bold       : Object = null,
											  $italic     : Object = null,
											  $undetline  : Object = null,
											  $align      : String = null,
											  $autosize   : String = null,
											  $x          : int    = 0,
											  $y          : int    = 0,
											  $initText   : String = null 
											) : TextField 
		{
			var textField  : TextField  = new TextField();
			var textFormat : TextFormat = new TextFormat($font, $size, $color, $bold, $italic, $undetline, null, null, $align);
			  
			textField.defaultTextFormat = textFormat;
			textField.multiline         = ($multiline  == null ? false : $multiline);
			textField.selectable        = ($selectable == null ? false : $selectable);
			textField.autoSize          = ($autosize   == null ? TextFieldAutoSize.NONE : $autosize);
			textField.x                 = $x;
			textField.y                 = $y;
			  
			if ($wordWrap != null) {
				textField.wordWrap = $wordWrap;
			}
			  
			if ($initText != null) {
				textField.text = $initText;  
			}
			return textField;
		}

	}
	
}