package  
{
	import starling.display.Sprite;

	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class Graphics extends Sprite
	{
		public function Graphics() 
		{

		}
		
		public function draw(physicsObject:BaseCircle) : void
		{
			graphics.clear();
			graphics.beginFill(physicsObject.color, 1);
			graphics.drawCircle(physicsObject.position.x, 
								physicsObject.position.y, 
								physicsObject.radius);
			graphics.endFill();
		}
		
	}

}