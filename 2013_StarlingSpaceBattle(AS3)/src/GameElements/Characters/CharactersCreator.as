package GameElements.Characters 
{
	import GameElements.Characters.EvilGuys.FastEvilGuy;
	import GameElements.Characters.EvilGuys.MiddleEvilGuy;
	import GameElements.Characters.EvilGuys.SlowEvilGuy;
	import starling.display.DisplayObjectContainer;
	/**
	 * ...
	 * @author werewolf
	 */
	public class CharactersCreator 
	{
		
		public static const HERO :uint = 0;
		public static const SLOW_EVIL_GUY:uint = 1;
		public static const MIDDLE_EVIL_GUY:uint = 2;
		public static const FAST_EVIL_GUY:uint = 3;
		
		public function CharactersCreator() 
		{
			
		}
		
		public function create($characterType:int, $parent:DisplayObjectContainer, $x:int = 0, $y:int = 0):CharacterBase {
			var character:CharacterBase = this.createCharacter($characterType);		
			character.draw();
			character.x = $x - (character.width / 2);
			character.y = $y - (character.height / 2);
			$parent.addChild(character);
			return character;
		}
		
		private function createCharacter($characterType:int):CharacterBase 
		{
			switch ($characterType) {
				case HERO:
					return new Hero();
				case SLOW_EVIL_GUY:
					return new SlowEvilGuy();
				case MIDDLE_EVIL_GUY:
					return new MiddleEvilGuy();
				case FAST_EVIL_GUY:
					return new FastEvilGuy();
				default:
					trace("Invalid kind of character");
					return null;			
			}
			return null;
		}
		
	}

}