package Preloader
{
	import Constants.GameEvents;
	import flash.ui.Mouse;
	import starling.display.Image;
	import starling.display.Sprite;
	import starling.events.Event;
	import starling.events.Touch;
	import starling.events.TouchEvent;
	import starling.events.TouchPhase;
	import starling.text.TextField;
	import starling.textures.Texture;
	import starling.utils.HAlign;
	import starling.utils.VAlign;
	
	/**
	 * Крутилка
	 * 
	 * @author WereWolf
	 */
	public class LoaderClip extends Sprite 
	{			
		private var loader:Sprite;
		private var loaderTest:Image;
		
		[Embed(source="../../assets/LOADER_CLIP.png")]
		public static var LOADER_CLIP: Class;
		
		public function LoaderClip() 
		{
			var imgClass: Class = LoaderClip["LOADER_CLIP"];	
			var texture:Texture = Texture.fromBitmap(new imgClass());
			loaderTest = new Image(texture);
			loaderTest.x = -loaderTest.width / 2;
			loaderTest.y = -loaderTest.height / 2;
			loader = new Sprite();
			loader.addChild(loaderTest);
			
			addChild(loader);
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
			addEventListener(Event.REMOVED_FROM_STAGE, destructor);			
		}		
		
		public function changeToButton():void 
		{
			removeEventListener(Event.ENTER_FRAME, onEnterFrame);
			touchable = true;
			loader.rotation = 0;
			var tf:TextField = GameUtils.createTextField("START", 24, 0x00ff00, true, VAlign.CENTER, HAlign.CENTER, this.width, this.height);
			tf.x = loaderTest.x + (loaderTest.width - tf.width) / 2;
			tf.y = loaderTest.y;
			loader.removeChild(loaderTest);
			tf.border = true;
			loader.addChild(tf);
			addEventListener(TouchEvent.TOUCH, onTouch);
		}
		
		private function onTouch(e:TouchEvent):void 
		{
			var touch:Touch = e.getTouch(stage);
			
			if (touch && touch.phase == TouchPhase.ENDED) {
				StarlingGlobalEventDispatcher.instance.dispatchEventWith(GameEvents.PRESS_BUTTON_START);
				Mouse.hide();
			}
		}
		
		override public function get height():Number 
		{
			return loaderTest.height;
		}	
		
		override public function get width():Number 
		{
			return loaderTest.width;
		}

		private function onEnterFrame(e:Event):void 
		{			
			loader.rotation += 6;
		}
	
		private function destructor(e:Event):void 
		{			
			removeEventListener(Event.ENTER_FRAME, onEnterFrame);			
			removeEventListener(Event.REMOVED_FROM_STAGE, destructor);
		}
		
	}

}