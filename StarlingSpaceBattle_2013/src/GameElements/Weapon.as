package  GameElements
{
	import GameElements.Bullets.BulletBase;
	import GameElements.Bullets.EvilGuysSimpleBullet;
	import GameElements.Bullets.HeroSimpleBullet;
	import starling.display.DisplayObjectContainer;
	import starling.display.Sprite;
	/**
	 * Фабрика патронов
	 * @author werewolf
	 */
	public class Weapon extends Sprite
	{
		public static const EVIL_SIMPLE_BULLET :uint = 0;
		public static const HERO_SIMPLE_BULLET :uint = 1;
		
		public function Weapon() 
		{
			
		}
		
		public function fire(weaponType:int, $parent:DisplayObjectContainer, $x:int = 0, $y:int = 0):BulletBase {
			var bullet:BulletBase = createBullet(weaponType);
			bullet.x = $x - bullet.width / 2;
			bullet.y = $y;
			$parent.addChild(bullet);
			return bullet;
		}
		
		protected function createBullet(weaponType:int):BulletBase
		{
			switch (weaponType) {
				case EVIL_SIMPLE_BULLET:
					return new EvilGuysSimpleBullet();
				case HERO_SIMPLE_BULLET:
					return new HeroSimpleBullet();
				default:
					trace("Invalid kind of bullet");
					return null;			
			}
			return null;
		}
		
	}

}