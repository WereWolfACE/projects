package GameElements.Characters.EvilGuys 
{
	import Constants.AnimationKeys;
	import Constants.GameConstants;
	import GameElements.Weapon;
	/**
	 * ...
	 * @author werewolf
	 */
	public class SlowEvilGuy extends EvilGuyBase 
	{

		public function SlowEvilGuy() 
		{
			super();
		}
		
		override protected function init():void 
		{			
			_animationKey = AnimationKeys.EVIL_GUY_1;
			_hp = 80;
			_maxHp = _hp;
			_currentWeapon = Weapon.EVIL_SIMPLE_BULLET;
			shotSpeed = GameUtils.randRange(GameConstants.SHOT_SPEED_SLOW_MIN, GameConstants.SHOT_SPEED_SLOW_MAX);
			super.init();
		}
	}
}