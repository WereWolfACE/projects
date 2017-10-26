package  GameElements.Bullets
{
	import Constants.AnimationKeys;
	import Constants.GameObjectsTypes;
	/**
	 * ...
	 * @author werewolf
	 */
	public class EvilGuysSimpleBullet extends BulletBase
	{
		
		public function EvilGuysSimpleBullet() 
		{
			super();
		}
		
		override protected function init():void 
		{
			_animationKey = AnimationKeys.BULLET;
			_typeToCollision = GameObjectsTypes.EVIL_BULLET;
			_speed = 3;
			_damage = 10;
		}
		
	}

}