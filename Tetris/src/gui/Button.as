package gui
{
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	/**
	 * ...
	 * @author Marshalova Valeriya 
	 */
	public class Button extends Sprite 
	{
		private var text:TextField;
		private var onClickFunction:Function;
		
		public function Button($text:String, $onClickFunction:Function) {
			addChild(View.createWindowBorder(200, 60));
			
			text = View.createLabel("Tahoma", 30, 0xffffff, false, true, false, true, false, false, "center", null, 0, 0, $text);
			text.width = 200;
			text.height = 60;
			addChild(text);
			
			buttonMode = true;
			
			onClickFunction = $onClickFunction;
			addEventListener(MouseEvent.CLICK, onClick);
			
		}
		
		private function onClick(e:MouseEvent):void 
		{
			if (onClickFunction != null) 
				onClickFunction(e);
		}
		
		public function destroy() {
			removeEventListener(MouseEvent.CLICK, onClick);
			if(parent.contains(this)) parent.removeChild(this);
		}
		
		
	}
	
}