package  
{
	import flash.geom.Point;
	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class Utils 
	{
		// проверка, лежат ли две точки в одной полуплоскости относительно прямой, заданной двумя точками
		static public function isPointsAtSamePlain(point1:Point, point2:Point, beginLine:Point, endLine:Point):Boolean 
		{
			var f1:Number = (point1.x - beginLine.x) / (endLine.x - beginLine.x) - 
							(point1.y - beginLine.y) / (endLine.y - beginLine.y);
			var f2:Number = (point2.x - beginLine.x) / (endLine.x - beginLine.x) - 
							(point2.y - beginLine.y) / (endLine.y - beginLine.y);
			if (f1 == 0 && f2 == 0) return true;
			if ((f1 > 0 && f2 > 0) || (f1 < 0 && f2 < 0)) return true;
			return false;
		}

		// длина вектора (модуль вектора)
		static public function getVectorLength(vector:Point):Number {
			return Math.sqrt(Math.pow(vector.x, 2) + Math.pow(vector.y, 2));
		}
		
		// скалярное произведение векторов
		static public function getScalar(vector1:Point, vector2:Point):Number {
			return vector1.x * vector2.x + vector1.y * vector2.y;
		}
		
		// для векторного произведения векторов в двумерном пространстве
		static public function getPseudoScalar(vector1:Point, vector2:Point):Number {
			return vector1.x * vector2.y - vector1.y * vector2.x;
		}
		
		// создать вектор из двух точек
		static public function getVector(point1:Point, point2:Point):Point {
			return new Point(point2.x - point1.x, point2.y - point1.y);
		}
		
		// получить нормаль из двух точек
		static public function getNormal(point1:Point, point2:Point):Point {			
			var normal : Point = new Point();
			normal.x = point1.y - point2.y;
			normal.y = point2.x - point1.x;
			var length:Number = getVectorLength(normal);
			normal.x /= length;
			normal.y /= length;
			
			return normal;
		}
		
		//расстояние от точки до линии
		static public function getDistanseFromPointToLine(point : Point, beginLine : Point, endLine : Point) : Number {
			var vectorToPoint : Point = getVector(point, beginLine);
			var lineVector : Point = getVector(beginLine, endLine);
			var distance:Number = Math.abs(getPseudoScalar(vectorToPoint, lineVector) / getVectorLength(lineVector));
			
			return distance;			
		}
		
		// получить расстояние от точки до точки
		static public function getDistanseFromPointToPoint(point1:Point, point2:Point):Number 
		{
			var vector:Point = getVector(point1, point2);
			var length:Number = getVectorLength(vector);
			
			return length;
		}
		
		// получить нормаль из двух точек
		static public function normalize(vector:Point):Point {			
			var length:Number = getVectorLength(vector);
			vector.x /= length;
			vector.y /= length;
			
			return vector;
		}
		
		// вычисление площади пересечения двух окружностей
		static public function intersectionSquare(circle1:BaseCircle, circle2:BaseCircle):Number 
		{
			var dist:Number = getDistanseFromPointToPoint(circle1.position, circle2.position);
			var f1:Number = 2 * Math.acos((Math.pow(circle1.radius, 2) - Math.pow(circle2.radius, 2) + Math.pow(dist, 2)) / (2 * circle1.radius * dist));
			var f2:Number = 2 * Math.acos((Math.pow(circle2.radius, 2) - Math.pow(circle1.radius, 2) + Math.pow(dist, 2)) / (2 * circle2.radius * dist));
			var squre1:Number = (Math.pow(circle1.radius, 2) * (f1 - Math.sin(f1))) / 2;
			var squre2:Number = (Math.pow(circle2.radius, 2) * (f2 - Math.sin(f2))) / 2;
			return squre1 + squre2;
		}
		
		//сравнение двух окружностей (для сортировки)
		public static function sortCircles(a:BaseCircle, b:BaseCircle):int 
		{
			if (a.radius < b.radius) return -1;
			else if (a.radius > b.radius) return 1;
			else return 0;
		}
		
		public function Utils() 
		{
			throw new Error("You can’t create instance of VectorUtils. All its methods and properties are static");
		}
		
	}

}