package it.gov.itisdalmine.automatedgreenhouse;

import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.CorsRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;

@Configuration
public class CorsConfig extends WebMvcConfigurerAdapter {

	@Override
	public void addCorsMappings(CorsRegistry registry) {
		System.out.println("Sina");
		//Allow any origin with any mapping
		registry.addMapping("/**").allowedOrigins("*");

	}	
}