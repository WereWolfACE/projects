package  
{
	import flash.geom.Point;
	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class PlayerCircle extends BaseCircle 
	{
		public function PlayerCircle() 
		{

		}
		
		public function setMoveVector(moveVector : Point, duration : int ):void {			
			var length : Number = Utils.getVectorLength(moveVector); 
			super.moveVector.x = moveVector.x / length;  
			super.moveVector.y = moveVector.y / length; 
			duration = (duration > BaseCircle.MAX_TIME) ? BaseCircle.MAX_TIME : duration;
			super.speed = Math.round(BaseCircle.MAX_SPEED / BaseCircle.MAX_TIME * duration);			
		}
		
		
		
	}

}