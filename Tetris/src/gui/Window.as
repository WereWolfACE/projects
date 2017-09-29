package gui 
{
	import flash.display.Sprite;
	import flash.text.TextField;
	/**
	 * ...
	 * @author Marshalova Valeriya 
	 */
	public class Window extends Sprite 
	{
		private var titleText:TextField;
		private var messageText:TextField;
		
		public function Window() 
		{
			addChild(View.createWindowBorder(250, 90));
			
			titleText = View.createLabel("Tahoma", 18, 0xffffff, false, true, false, true, false, false, "center", null, 0, 0, "");
			titleText.y = 5;
			titleText.width = 250;
			titleText.height = 25;
			
			messageText = View.createLabel("Tahoma", 16, 0xeeeeee, false, true, false, true, false, false, "center", null, 0, 0, "");
			messageText.width = 250;
			messageText.y = titleText.y + titleText.height + 5;
			
			addChild(titleText);
			addChild(messageText);	
		}
		
		public function close():void
		{
			if(parent && parent.contains(this))
				parent.removeChild(this);
		}
		
		public function set titleTextValue(value:String):void 
		{
			titleText.text = value;
		}
		
		public function set messageTextValue(value:String):void 
		{
			messageText.text = value;
		}
		
	}

}