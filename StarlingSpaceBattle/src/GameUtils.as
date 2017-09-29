package  
{
	import flash.text.TextFormat;
	import starling.text.TextField;
	import starling.utils.HAlign;
	import starling.utils.VAlign;
	/**
	 * ...
	 * @author werewolf
	 */
	public class GameUtils 
	{
		
		public function GameUtils() 
		{
			
		}
		
		static public function randRange($minNum:Number, $maxNum:Number):Number 
        {
            return (Math.floor(Math.random() * ($maxNum - $minNum + 1)) + $minNum);
        }
		
		public static function createTextField($text:String, $size:int = 12, $color:int = 0x0, $bold:Boolean = false, $vAlign:String = "left", $hAlign:String = "left", $width:int = 200, $height:int = 60):TextField
		{
			var tmp:TextField = new TextField($width, $height, $text, 'Tahoma', $size,  $color, $bold);
			tmp.hAlign = $hAlign;
			tmp.vAlign = $vAlign;			
			return tmp;
		}
		
	}

}