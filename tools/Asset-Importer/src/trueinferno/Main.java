package trueinferno;

import java.io.File;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {
	private static int NUM_ARGS = 0;
	private static final int ARG_PROJECT_ROOT = NUM_ARGS++;
	private static final int ARG_ASSETS_ROOT = NUM_ARGS++;
	
	public static void main(String[] args) {
		Logger logger = Logger.getGlobal();
		
		if(args.length != NUM_ARGS){
			System.out.println("Argument error!");
			System.out.println("Syntax: <PROYECT_ROOT> <ASSETS_ROOT>");
			System.out.println("<PROYECT_ROOT> is the folder containing 'Exes', 'Projects', etc.");
			System.out.println("<ASSETS_ROOT> is the folder containing 'Modelos3D', etc.");
			return;
		}
		
		logger.info("Starting import...");
		
		try{
			File projectRoot = new File(args[ARG_PROJECT_ROOT]);
			File assetRoot = new File(args[ARG_ASSETS_ROOT]);
			
			String reportFileName = "out" + File.separator + "report_" + Util.getTimeStamp() + ".txt";
			logger.info("Report file: " + reportFileName);
			File reportFile = new File(reportFileName);
			
			AssetImporter importer = new AssetImporter(projectRoot, assetRoot, reportFile);
			importer.importAssets();
			
			logger.info("Import finished OK");
		}
		
		catch(Exception e){
			logger.log(Level.SEVERE, "Import finished with errors. Please check logs and re-run", e);
		}
		
		logger.info("Exit");
	}
}
