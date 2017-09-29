package  GameElements.Bullets
{
	import Constants.AnimationKeys;
	import Constants.GameObjectsTypes;
	/**
	 * ...
	 * @author werewolf
	 */
	public class HeroSimpleBullet extends BulletBase
	{
		
		public function HeroSimpleBullet() 
		{
			super();
		}
		
		override protected function init():void 
		{
			_animationKey = AnimationKeys.BULLET;
			_typeToCollision = GameObjectsTypes.HERO_BULLET;
			_speed = -10;
			_damage = 10;
		}
		
	}

}