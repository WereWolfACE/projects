package gui 
{
	import flash.display.Sprite;
	import flash.text.TextField;
	/**
	 * ...
	 * @author Marshalova Valeriya 
	 */
	public class UserInfoView extends Sprite
	{
		private var _expText:TextField;
		private var _expValue:TextField;
		
		private var _nextExpText:TextField;
		private var _nextExtValue:TextField;
		
		private var _levelText:TextField;
		private var _levelValue:TextField;
		
		public function UserInfoView() 
		{
			_expText = View.createLabel("Tahoma", 14, 0x0, false, true, false, false, false, true, "left", null, 0, 0, Config.getLocalizationText(Config.LOCAL_TEXT_EXP));
			_expText.width = Config.PANEL_WIDTH;
			_expValue = View.createDefaultTextField("0");
			_expValue.y = _expValue.y + _expValue.height + 5;			
			_expValue.width = Config.PANEL_WIDTH + 20;
			
			_levelText = View.createLabel("Tahoma", 14, 0x0, false, true, false, false, false, true, "left", null, 0, 0, Config.getLocalizationText(Config.LOCAL_TEXT_LEVEL));
			_levelText.width = Config.PANEL_WIDTH - 10;
			_levelText.y = 60;
			_levelValue = View.createDefaultTextField("0");
			_levelValue.width = 20;
			_levelValue.y = _levelText.y;
			_levelValue.x = _levelText.x + _levelText.width + 5;
			
			addChild(_expText);
			addChild(_expValue);
			addChild(_levelText);
			addChild(_levelValue);
		}
		
		public function set expTextValue(value:String):void 
		{
			_expValue.text = value;
		}
		
		public function set levelTextValue(value:String):void 
		{
			_levelValue.text = value;
		}
		
	}

}