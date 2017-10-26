package Constants
{
	/**
	 * ...
	 * @author werewolf
	 */
	public class GameConstants 
	{
		static public const COUNT_ENEMIES:int = 100;
		
		//названия анимаций в спрайт листах
		static public const HIT:String = "hit";
		static public const IDLE:String = "idle";
		static public const MOVE:String = "move";
		static public const DESTROY:String = "destroy";
		static public const FIRE:String = "fire";
		
		//скорость выстрелов врагов в милисекундах
		static public const SHOT_SPEED_SLOW_MAX:Number = 10000;
		static public const SHOT_SPEED_SLOW_MIN:Number = 8000;
		static public const SHOT_SPEED_MIDDLE_MAX:Number = 8000;
		static public const SHOT_SPEED_MIDDLE_MIN:Number = 5000;
		static public const SHOT_SPEED_FAST_MAX:Number = 5000;
		static public const SHOT_SPEED_FAST_MIN:Number = 3000;	
		
		//время паузы между появлением врагов на поле
		static public const MIN_ENEMY_CREATE_TIME:int = 500;
		static public const MAX_ENEMY_CREATE_TIME:int = 1000;
		
		//количество убитых врагов, необходимое для появления нового типа врага
		static public const COUNT_KILLED_FOR_MIDDLE_SPEED:int = 15;
		static public const COUNT_KILLED_FOR_FAST_SPEED:int = 25;
		
		//интервал в милисекундах между пулями героя, когда тот стреляет очередью
		static public const HERO_MULTISHOT_SPEED:int = 60;
		
		
		//для оптимизации храним высоту объектов
		static public const CHARACTER_WIDTH:int = 40;
		static public const CHARACTER_HEIGHT:int = 40;
		static public const BULLET_WIDTH:int = 10;
		static public const BULLET_HEIGHT:int = 10;
		
		public function GameConstants() 
		{
			
		}
		
	}

}