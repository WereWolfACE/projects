package  
{
	import flash.media.Sound;
	import flash.media.SoundChannel;
	import flash.media.SoundTransform;
	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class SoundController 
	{
		
		public function SoundController() 
		{
			
		}
		
		public static function playSound(sound:Sound, volume:Number = 1, loop:int = 0):void
		{
			var soundTransform:SoundTransform = new SoundTransform(volume);
			var soundChanel:SoundChannel = sound.play(0, loop, soundTransform);	
		}
		
	}

}