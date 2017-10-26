package GameElements.Characters.EvilGuys 
{
	import Constants.AnimationKeys;
	import Constants.GameConstants;
	import GameElements.Weapon;
	/**
	 * ...
	 * @author werewolf
	 */
	public class MiddleEvilGuy extends EvilGuyBase 
	{

		public function MiddleEvilGuy() 
		{
			super();
		}
		
		override protected function init():void 
		{			
			_animationKey = AnimationKeys.EVIL_GUY_2;
			_hp = 50;
			_maxHp = _hp;
			_currentWeapon = Weapon.EVIL_SIMPLE_BULLET;
			shotSpeed = GameUtils.randRange(GameConstants.SHOT_SPEED_MIDDLE_MIN, GameConstants.SHOT_SPEED_MIDDLE_MAX);
			super.init();
		}
	}
}