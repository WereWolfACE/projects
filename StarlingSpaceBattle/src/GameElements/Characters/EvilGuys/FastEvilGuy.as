package GameElements.Characters.EvilGuys 
{
	import Constants.AnimationKeys;
	import Constants.GameConstants;
	import GameElements.Weapon;
	/**
	 * ...
	 * @author werewolf
	 */
	public class FastEvilGuy extends EvilGuyBase 
	{

		public function FastEvilGuy() 
		{
			super();
		}
		
		override protected function init():void 
		{			
			_animationKey = AnimationKeys.EVIL_GUY_3;
			_hp = 30;
			_maxHp = _hp;
			_currentWeapon = Weapon.EVIL_SIMPLE_BULLET;
			shotSpeed = GameUtils.randRange(GameConstants.SHOT_SPEED_FAST_MIN, GameConstants.SHOT_SPEED_FAST_MAX);
			super.init();
		}
	}
}