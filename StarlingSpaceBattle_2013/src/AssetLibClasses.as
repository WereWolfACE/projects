package  {
	internal class AssetLibClasses {
		// Embed the Atlas XML & Texture

		[Embed(source="../assets/bullet.png")]
		public static const bullet: Class;
		[Embed(source="../assets/bullet.xml", mimeType="application/octet-stream")]
		public static const bullet_xml: Class;   
		
		[Embed(source="../assets/hero.png")]
		public static const hero: Class;
		[Embed(source="../assets/hero.xml", mimeType="application/octet-stream")]
		public static const hero_xml: Class; 
		
		
		[Embed(source="../assets/evilGuy1.png")]
		public static const evilGuy1: Class;
		[Embed(source="../assets/evilGuy1.xml", mimeType="application/octet-stream")]
		public static const evilGuy1_xml: Class; 
		
		[Embed(source="../assets/evilGuy2.png")]
		public static const evilGuy2: Class;
		[Embed(source="../assets/evilGuy2.xml", mimeType="application/octet-stream")]
		public static const evilGuy2_xml: Class; 
		
		[Embed(source="../assets/evilGuy3.png")]
		public static const evilGuy3: Class;
		[Embed(source="../assets/evilGuy3.xml", mimeType="application/octet-stream")]
		public static const evilGuy3_xml: Class; 
	}
}