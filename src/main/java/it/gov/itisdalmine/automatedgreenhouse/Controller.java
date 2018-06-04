package it.gov.itisdalmine.automatedgreenhouse;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class Controller {

	
	private char stato0 = 'f';
	private char stato1 = 'f';
	private char stato2 = 'f';
	
	@GetMapping("/stato")
	public char getStato0()
	{
		return stato0;
	}
	@PostMapping("/stato")
	public void setStato0(@RequestBody char newstate)
	{
		this.stato0=newstate;
	}
	
	
	@GetMapping("/stato1")
	public char getStato1()
	{
		return stato1;
	}
	@PostMapping("/stato1")
	public void setStato1(@RequestBody char newstate)
	{
		this.stato1=newstate;
	} 
	
	
	@GetMapping("/stato2")
	public char getStato2()
	{
		return stato2;
	}
	@PostMapping("/stato2")
	public void setStato2(@RequestBody char newstate)
	{
		this.stato2=newstate;
	} 
	
}
