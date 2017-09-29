package 
{
	import flash.geom.Point;
	/**
	 * ...
	 * @author Marshalova Valeriya 
	 */
	public class Config
	{		
		// path to xml with shapes declaration
		static public const SHAPES_XML_PATH	: String = "shapes.xml";
		// shapes xml has view
		/*  
		  	<Shapes>
				<Shape color="0xffffff">
					<Points>
						<Point x = "0" y = "0" />					
					</Points>
				</Shape>	
			</Shapes>
		*/		
		
		public static const FIELD_WIDTH 				: int = 15; 	// in cells
		public static const FIELD_HEIGHT 				: int = 25; 	// in cells
		public static const DEFAULT_SHAPE_POSITION_H	: int = 0;  	// in cells
		public static const DEFAULT_SHAPE_POSITION_W	: int = 6;		// in cells		
		public static const CELL_SIZE					: int = 20; 	// in pixels
		public static const DEFAULT_TIME				: int = 1000; 	// default time to shape down move, in milliseconds
		public static const DECREASE_TIME				: int = 50; 	// decrease each level time, in milliseconds
		
		public static const FIELD_COLOR		: uint = 0xFEFED6;				// field background color
		public static const BORDER_COLOR	: uint = 0x95004A;				// gui border color
		
		public static var EXP_TABLE : Array /*of Numbers*/ = [10, 100, 300, 700, 1500]; // indexes - collected rows count, values - exp points 
		
		// directions
		public static const QUICK_DOWN		: int = 0;
		public static const DOWN			: int = 1;
		public static const LEFT			: int = 2;
		public static const RIGHT			: int = 3;		
		public static const ROTATE			: int = 4;
		public static const BEGIN_POINTS	: int = 5;		
		
		public static const EMPTY : int = 0; // empty cell		
		
		public static var SHAPES : Array /*of Objects*/ = [ { color:FIELD_COLOR, points: [new Point(0, 0)] } ]; // array of shapes objects. 
																												// Properties: color, points
																												// Default value: empty cell
				
		public static const PANEL_WIDTH:int = 5 * CELL_SIZE; // in pixels
		
		
		//-------------------------------------------------
		// Localization
		//-------------------------------------------------
		public static const LOCAL_TEXT_LEVEL		:String = "level";
		public static const LOCAL_TEXT_NEXT_SHAPE	:String = "nextShape";
		public static const LOCAL_TEXT_EXP			:String = "exp";
		public static const LOCAL_TEXT_PAUSE		:String = "pause";
		public static const LOCAL_TEXT_START		:String = "start";
		public static const LOCAL_TEXT_UNPAUSE		:String = "unpause";
		public static const LOCAL_TEXT_LOSE_TITLE	:String = "loseTitle";
		public static const LOCAL_TEXT_LOSE_MESSAGE	:String = "loseMessage";
		
		private static var RU_LOCAL : Object = { level 		: "Уровень",
												nextShape 	: "Следующая фигура",
												exp 		: "Опыт",
												pause 		: "Пауза",
												start 		: "Начать",
												unpause		: "Чтобы продолжить игру нажмите ESC",
												loseTitle		: "Вы проиграли",
												loseMessage		: "Начать игру заново? (Y / N)" };
												
		private static var ENG_LOCAL : Object = { level 	: "Level",
												nextShape 	: "Next shape",
												exp 		: "Experience",
												pause 		: "Pause",
												start 		: "Start",
												unpause		: "Press ESC to continue",
												loseTitle	: "You are lose.",
												loseMessage	: "Start game again? (Y / N)" };
		private static var LANGUAGES : Object = { "ru" : RU_LOCAL, "eng" : ENG_LOCAL };
		public static const LOCALIZATION : String = "ru";
		
		public static function getLocalizationText($param:String) : String
		{
			return LANGUAGES[LOCALIZATION][$param];
		}
		
		public function Config() 
		{
			
		}	
		
	}
	
}