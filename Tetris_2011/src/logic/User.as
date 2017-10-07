package logic
{	
	/**
	 * ...
	 * @author Marshalova Valeriya
	 */
	public class User 
	{			
		private var _exp:int = 0;
		private var _level:int = 1;
		
		public function User() 
		{
			
		}
		
		/**
		 * add experience to user depending on collected rows count
		 * @param	$collectedRows
		 */
		public function setExp($collectedRows:int):void 
		{
			_exp += Config.EXP_TABLE[$collectedRows];			
			_level = int(_exp / expToNextLevel) == 1 ? _level + 1 : _level;
		}		
		
		//--------------------------------------------------------------------------
		//
		//  Getters&setters
		//
		//--------------------------------------------------------------------------
		
		/**
		 * experience to next level
		 * @return points of experience need to go to next level
		 */
		public function get expToNextLevel():int 
		{
			var nextLevel:int = _level + 1;
			return(nextLevel * (nextLevel + 1) * (nextLevel + 2) + 16 * nextLevel) * 20;						
		}	
		
		public function get exp():int 
		{
			return _exp;
		}			
		
		public function get level():int 
		{
			return _level;
		}
	}
	
}